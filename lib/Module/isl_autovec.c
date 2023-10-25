/*
 * Copyright 2023      zhaosiying12138@IAYJT_LAS
 *
 * Use of this software is governed by the MIT license
 *
 * Written by zhaosiying12138, Institute of Advanced YanJia
 *  Technology - LiuYueCity Academy of Science 
 */

#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <isl_ctx_private.h>
#include <isl_map_private.h>
#include <isl_aff_private.h>
#include <isl_space_private.h>
#include <isl/id.h>
#include <isl/set.h>
#include <isl/flow.h>
#include <isl_constraint_private.h>
#include <isl/polynomial.h>
#include <isl/union_set.h>
#include <isl/union_map.h>
#include <isl_factorization.h>
#include <isl/schedule.h>
#include <isl/schedule_node.h>
#include <isl_options_private.h>
#include <isl_vertices_private.h>
#include <isl/ast_build.h>
#include <isl/val.h>
#include <isl/ilp.h>
#include <isl_ast_build_expr.h>
#include <isl/options.h>

#define STR_MAX 10

struct zsy_dep_graph
{
	int n;
	char **node_names;
	int *edge;
	int *visited;
};

static void zsy_dep_graph_init(struct zsy_dep_graph *dep_graph, isl_union_set *domain)
{
	int n = isl_union_set_n_set(domain);
	dep_graph->n = n;
	dep_graph->node_names = (char **)malloc(n * sizeof(char *));
	dep_graph->edge = (int *)calloc(n * n, sizeof(int));
	isl_map_list *domain_list = isl_union_set_get_set_list(domain);
	for (int i = 0; i < n; i++) {
		isl_set *tmp_set = isl_set_list_get_at(domain_list, i);
		const char *tmp_name = isl_set_get_tuple_name(tmp_set);
		dep_graph->node_names[i] = (char *)malloc(STR_MAX * sizeof(char));
		strncpy(dep_graph->node_names[i], tmp_name, STR_MAX - 1);
//		printf("%s\n", dep_graph->node_names[i]);
	}
	dep_graph->visited = (int *)calloc(n, sizeof(int));
}

static void zsy_dep_graph_clear_visited(struct zsy_dep_graph *dep_graph)
{
	for (int i = 0; i < dep_graph->n; i++) {
		dep_graph->visited[i] = 0;
	}
}

static int zsy_dep_graph_get_node_id(struct zsy_dep_graph *dep_graph, const char *node_name)
{
	for (int i = 0; i < dep_graph->n; i++) {
		if (strncmp(dep_graph->node_names[i], node_name, STR_MAX - 1) == 0)
			return i;
	}
	return -1;
}

static void zsy_dep_graph_add_edge(struct zsy_dep_graph *dep_graph, int id1, int id2)
{
	dep_graph->edge[id1 * dep_graph->n + id2] = 1;
}

static int zsy_dep_graph_check_edge(struct zsy_dep_graph *dep_graph, int id1, int id2)
{
	return dep_graph->edge[id1 * dep_graph->n + id2];
}

static void zsy_dep_graph_dump(struct zsy_dep_graph *dep_graph)
{
	printf("Dependece Graph Dump:\n");
	for (int i = 0; i < dep_graph->n; i++) {
		for (int j = 0; j < dep_graph->n; j++) {
			if (zsy_dep_graph_check_edge(dep_graph, i, j)) {
				printf("add_edge(\"%s\", \"%s\");\n", dep_graph->node_names[i], dep_graph->node_names[j]);
			}
		}
	}
}

void zsy_compute_dep_graph(struct zsy_dep_graph *dep_graph, isl_union_set *domain, isl_union_map *dep_all)
{
	zsy_dep_graph_init(dep_graph, domain);

	isl_map_list *map_list = isl_union_map_get_map_list(dep_all);
	for (int i = 0; i < isl_map_list_n_map(map_list); i++) {
		isl_map *tmp_map = isl_map_list_get_map(map_list, i);
		const char *node_source_name = isl_map_get_tuple_name(tmp_map, isl_dim_in);
		const char *node_sink_name = isl_map_get_tuple_name(tmp_map, isl_dim_out);
		int source_id = zsy_dep_graph_get_node_id(dep_graph, node_source_name);
		int sink_id = zsy_dep_graph_get_node_id(dep_graph, node_sink_name);
		zsy_dep_graph_add_edge(dep_graph, source_id, sink_id);
	}
	printf("\n");
	zsy_dep_graph_dump(dep_graph);
}

static int zsy_dep_graph_check_recurrence_call(struct zsy_dep_graph *dep_graph, int start_node_id)
{
	if (dep_graph->visited[start_node_id] == 1)
		return 1;
	dep_graph->visited[start_node_id] = 1;
	for (int i = 0; i < dep_graph->n; i++) {
		if (zsy_dep_graph_check_edge(dep_graph, start_node_id, i) == 1) {
			//printf("Check %s -> %s\n", dep_graph->node_names[start_node_id], dep_graph->node_names[i]);
			if (zsy_dep_graph_check_recurrence_call(dep_graph, i) == 1)
				return 1;
		}
	}
	dep_graph->visited[start_node_id] = 0;
	return 0;
}

int zsy_dep_graph_check_recurrence(struct zsy_dep_graph *dep_graph)
{
	zsy_dep_graph_clear_visited(dep_graph);
	for (int i = 0; i < dep_graph->n; i++) {
		if (zsy_dep_graph_check_recurrence_call(dep_graph, i) == 1)
			return 1;
	}
	return 0;
}

static int zsy_compute_max_common_loops(__isl_keep isl_basic_map *bmap, __isl_keep isl_union_map *S)
{
	isl_basic_set *bmap_domain = isl_basic_map_domain(isl_basic_map_copy(bmap));
	isl_size sched1_dim = isl_basic_set_dim(bmap_domain, isl_dim_set);
	isl_union_map *sched_domain = isl_union_map_intersect_domain(isl_union_map_copy(S), isl_union_set_from_basic_set(bmap_domain));
	isl_union_set *sched_domain_range = isl_union_map_range(sched_domain);
	isl_basic_set_list *sched1_list = isl_union_set_get_basic_set_list(sched_domain_range);
	assert(isl_basic_set_list_n_basic_set(sched1_list) == 1);
	isl_basic_set *sched1 = isl_basic_set_list_get_basic_set(sched1_list, 0);

	isl_basic_set *bmap_range = isl_basic_map_range(isl_basic_map_copy(bmap));
	isl_size sched2_dim = isl_basic_set_dim(bmap_range, isl_dim_set);
	isl_union_map *sched_range = isl_union_map_intersect_domain(isl_union_map_copy(S), isl_union_set_from_basic_set(bmap_range));
	isl_union_set *sched_range_range = isl_union_map_range(sched_range);
	isl_basic_set_list *sched2_list = isl_union_set_get_basic_set_list(sched_range_range);
	assert(isl_basic_set_list_n_basic_set(sched2_list) == 1);
	isl_basic_set *sched2 = isl_basic_set_list_get_basic_set(sched2_list, 0);
	
	isl_size sched_dim_min = sched1_dim < sched2_dim ? sched1_dim : sched2_dim;
	//printf("sched1_dim = %d, sched2_dim = %d\n", sched1_dim, sched2_dim);

	int max_common_loops = 0;
	isl_int tmp_val1, tmp_val2;
	isl_int_init(tmp_val1);
	isl_int_init(tmp_val2);
	for (int i = 0; i < sched_dim_min * 2; i += 2) {
		assert(isl_basic_set_plain_dim_is_fixed(sched1, i, &tmp_val1));
		assert(isl_basic_set_plain_dim_is_fixed(sched2, i, &tmp_val2));
		if (isl_int_eq(tmp_val1, tmp_val2)) {
			max_common_loops++;
			continue;
		} else {
			break;
		}
	}

	return max_common_loops;
}

static int zsy_compute_loop_carried(__isl_keep isl_basic_map *bmap, __isl_keep isl_union_map *S)
{
	isl_size common_loop_size = zsy_compute_max_common_loops(bmap, S);
	isl_basic_map *bmap_tmp = isl_basic_map_copy(bmap);
	isl_size dim_in_size = isl_basic_map_dim(bmap_tmp, isl_dim_in);
	isl_size dim_out_size = isl_basic_map_dim(bmap_tmp, isl_dim_out);
	isl_int tmp_val;
	isl_int_init(tmp_val);
	int i;

	bmap_tmp = isl_basic_map_project_out(bmap_tmp, isl_dim_in,  common_loop_size, dim_in_size - common_loop_size);
	bmap_tmp = isl_basic_map_project_out(bmap_tmp, isl_dim_out, common_loop_size, dim_out_size - common_loop_size);
	isl_basic_set *delta = isl_basic_map_deltas(bmap_tmp);
	//isl_basic_set_dump(delta);

	for (i = 0; i < common_loop_size; i++) {
		if (isl_basic_set_plain_dim_is_fixed(delta, i, &tmp_val)) {
			if (isl_int_is_zero(tmp_val)) {
				continue;
			}
		}
		break;
	}
	if (i == common_loop_size)
		i = -1;
	else
		++i;
	isl_basic_map_dump(bmap);
	printf("max_common_loops = %d, loop_carried = %d\n", common_loop_size, i);

	isl_basic_set_free(delta);
	return i;
}

static void zsy_dump_dependence(__isl_keep isl_union_map *dep, __isl_keep isl_union_map *S)
{
	isl_map_list *map_list = isl_union_map_get_map_list(dep);
	for (int i = 0; i < isl_map_list_n_map(map_list); i++) {
		isl_map *tmp_map = isl_map_list_get_map(map_list, i);
		isl_basic_map_list *tmp_bmap_list = isl_map_get_basic_map_list(tmp_map);
		for (int j = 0; j < isl_basic_map_list_n_basic_map(tmp_bmap_list); j++) {
			isl_basic_map *tmp_bmap = isl_basic_map_list_get_basic_map(tmp_bmap_list, j);
			zsy_compute_loop_carried(tmp_bmap, S);
		}
	}
}

int zsy_pdg_calculate_ddg(isl_ctx *ctx)
{
	const char *d, *w, *r, *s;
	isl_union_set *D, *delta;
	isl_union_map *W, *R, *W_rev, *R_rev, *S, *S_le_S, *S_lt_S;
	isl_union_map *dep_raw, *dep_war, *dep_waw, *dep_all;
	struct zsy_dep_graph dep_graph;

	d = "[N] -> { S1[i] : 0 <= i < N; S2[i] : 0 <= i < N; S3[i] : 0 <= i < N; S4[i] : 0 <= i < N; "
				" S5[i] : 0 <= i < N; S6[i] : 0 <= i < N; S7[i] : 0 <= i < N; S8[i] : 0 <= i < N; }";
	w = "[N] -> { S3[i] -> A[i]; S5[i] -> T[]; S6[i] -> T[]; S7[i] -> B[i]; S8[i] -> C[i] }";
	r = "[N] -> { S1[i] -> A[i]; S2[i] -> A[i]; S2[i] -> B[i]; S3[i] -> B[i]; S4[i] -> A[i]; S4[i] -> T[]; "
		" S5[i] -> A[i]; S5[i] -> B[i]; S5[i] -> T[]; S6[i] -> A[i]; S6[i] -> B[i]; S6[i] -> T[]; "
		" S7[i] -> A[i]; S8[i] -> B[i]; S8[i] -> C[i] }";
	s = "[N] -> { S1[i] -> [0, i, 0]; S2[i] -> [0, i, 1]; S3[i] -> [0, i, 2]; S4[i] -> [0, i, 3]; "
				" S5[i] -> [0, i, 4]; S6[i] -> [0, i, 5]; S7[i] -> [0, i, 6]; S8[i] -> [0, i, 7]; }";

	D = isl_union_set_read_from_str(ctx, d);
	W = isl_union_map_read_from_str(ctx, w);
	R = isl_union_map_read_from_str(ctx, r);
	S = isl_union_map_read_from_str(ctx, s);

	W = isl_union_map_intersect_domain(W, isl_union_set_copy(D));
	R = isl_union_map_intersect_domain(R, isl_union_set_copy(D));
	S = isl_union_map_intersect_domain(S, isl_union_set_copy(D));

	W_rev = isl_union_map_reverse(isl_union_map_copy(W));
	R_rev = isl_union_map_reverse(isl_union_map_copy(R));
	S_lt_S = isl_union_map_lex_lt_union_map(isl_union_map_copy(S), isl_union_map_copy(S));
	S_le_S = isl_union_map_lex_le_union_map(isl_union_map_copy(S), isl_union_map_copy(S));
	dep_raw = isl_union_map_apply_range(isl_union_map_copy(W), isl_union_map_copy(R_rev));
	dep_raw = isl_union_map_intersect(dep_raw, isl_union_map_copy(S_lt_S));
	dep_waw = isl_union_map_apply_range(isl_union_map_copy(W), isl_union_map_copy(W_rev));
	dep_waw = isl_union_map_intersect(dep_waw, isl_union_map_copy(S_lt_S));
	dep_war = isl_union_map_apply_range(isl_union_map_copy(R), isl_union_map_copy(W_rev));
	dep_war = isl_union_map_intersect(dep_war, isl_union_map_copy(S_lt_S));

	printf("\nRAW Dependence:\n");
	zsy_dump_dependence(dep_raw, S);
	printf("\nWAW Dependence:\n");
	zsy_dump_dependence(dep_waw, S);
	printf("\nWAR Dependence:\n");
	zsy_dump_dependence(dep_war, S);

	dep_all = isl_union_map_union(isl_union_map_copy(dep_raw), isl_union_map_copy(dep_waw));
	dep_all = isl_union_map_union(dep_all, dep_war);
	zsy_compute_dep_graph(&dep_graph, D, dep_all);
	int is_recurrence = zsy_dep_graph_check_recurrence(&dep_graph);
	printf("Check Recurrence: %d\n", is_recurrence);
	return 0;
}

int main(int argc, char **argv)
{
	int i;
	struct isl_ctx *ctx;
	struct isl_options *options;

	options = isl_options_new_with_defaults();
	assert(options);
	argc = isl_options_parse(options, argc, argv, ISL_ARG_ALL);
	ctx = isl_ctx_alloc_with_options(&isl_options_args, options);

	printf("PDG-AutoVectorization Demo written by zhaosiying12138@Institute of Advanced YanJia"
				" Technology, LiuYueCity Academy of Science\n");
	zsy_pdg_calculate_ddg(ctx);

	isl_ctx_free(ctx);
	return 0;
error:
	isl_ctx_free(ctx);
	return -1;
}
