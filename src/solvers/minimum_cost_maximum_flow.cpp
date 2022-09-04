#include "stdafx.h"

#include <array>
#include <queue>
#include <fmt/core.h>

#include "instruction.h"
#include "instruction_cost_calculator.h"
#include "similarity_checker.h"
#include "solver_registry.h"

namespace {
  //
// Minimum Cost Maximum Flow (Tomizawa, Edmonds-Karp's successive shortest path)
//
// Description:
//   Given a directed graph G = (V,E) with nonnegative capacity c and cost w.
//   The algorithm find a maximum s-t flow of G with minimum cost.
// 
// Algorithm:
//   Tomizawa (1971), and Edmonds and Karp (1972)'s 
//   successive shortest path algorithm,
//   which is also known as the primal-dual method.
//
// Complexity:
//   O(F m log n), where F is the amount of maximum flow.
// 
// References:
//   N. Tomizawa (1971):
//   On some techniques useful for solution of transportation network problems.
//   Networks, vol. pp. 173-194.
//
//   J. Edmonds and R.M. Karp (1972):
//   Theoretical improvements in algorithmic efficiency for network flow problems.
//   Journal of ACM, vol. 19, pp. 248-264.
//
// Historical Note:
//   The successive shortest path type algorithm was developped
//   independently by Jewell (1958), Iri (1960), and Busacker and Gowen (1961).
//   Later, Tomizawa (1971), and Edmonds and Karp (1972) independently 
//   suggested to use vertex potential in shortest path algorithm.
// 

  using namespace std;

#define fst first
#define snd second
#define all(c) ((c).begin()), ((c).end())
#define TEST(s) if (!(s)) { cout << __LINE__ << " " << #s << endl; exit(-1); }

  const long long INF = 99999999;
  struct graph {
    typedef int flow_type;
    typedef int64_t cost_type;
    struct edge {
      int src, dst;
      flow_type capacity, flow;
      cost_type cost;
      size_t rev;
    };
    vector<edge> edges;
    void add_edge(int src, int dst, flow_type cap, cost_type cost) {
      adj[src].push_back({ src, dst, cap, 0, cost, adj[dst].size() });
      adj[dst].push_back({ dst, src, 0, 0, -cost, adj[src].size() - 1 });
    }
    int n;
    vector<vector<edge>> adj;
    graph(int n) : n(n), adj(n) { }

    pair<flow_type, cost_type> min_cost_max_flow(int s, int t) {
      flow_type flow = 0;
      cost_type cost = 0;

      for (int u = 0; u < n; ++u) // initialize
        for (auto& e : adj[u]) e.flow = 0;

      vector<cost_type> p(n, 0);

      auto rcost = [&](edge e) { return e.cost + p[e.src] - p[e.dst]; };
      for (int iter = 0; ; ++iter) {
        vector<int> prev(n, -1); prev[s] = 0;
        vector<cost_type> dist(n, INF); dist[s] = 0;
        if (iter == 0) { // use Bellman-Ford to remove negative cost edges
          vector<int> count(n); count[s] = 1;
          queue<int> que;
          for (que.push(s); !que.empty(); ) {
            int u = que.front(); que.pop();
            count[u] = -count[u];
            for (auto& e : adj[u]) {
              if (e.capacity > e.flow && dist[e.dst] > dist[e.src] + rcost(e)) {
                dist[e.dst] = dist[e.src] + rcost(e);
                prev[e.dst] = e.rev;
                if (count[e.dst] <= 0) {
                  count[e.dst] = -count[e.dst] + 1;
                  que.push(e.dst);
                }
              }
            }
          }
        }
        else { // use Dijkstra 
          typedef pair<cost_type, int> node;
          priority_queue<node, vector<node>, greater<node>> que;
          que.push({ 0, s });
          while (!que.empty()) {
            node a = que.top(); que.pop();
            if (a.snd == t) break;
            if (dist[a.snd] > a.fst) continue;
            for (auto e : adj[a.snd]) {
              if (e.capacity > e.flow && dist[e.dst] > a.fst + rcost(e)) {
                dist[e.dst] = dist[e.src] + rcost(e);
                prev[e.dst] = e.rev;
                que.push({ dist[e.dst], e.dst });
              }
            }
          }
        }
        if (prev[t] == -1) break;

        for (int u = 0; u < n; ++u)
          if (dist[u] < dist[t]) p[u] += dist[u] - dist[t];

        function<flow_type(int, flow_type)> augment = [&](int u, flow_type cur) {
          if (u == s) return cur;
          edge& r = adj[u][prev[u]], & e = adj[r.dst][r.rev];
          flow_type f = augment(e.src, min(e.capacity - e.flow, cur));
          e.flow += f; r.flow -= f;
          return f;
        };
        flow_type f = augment(t, INF);
        flow += f;
        cost += f * (p[t] - p[s]);
      }
      return { flow, cost };
    }
  };
}

class MinimumCostMaximumFlow : public SolverBase {
public:
  struct Option : public OptionBase {
    void setOptionParser(CLI::App* app) override {
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  MinimumCostMaximumFlow() { }
  SolverOutputs solve(const SolverArguments& args) override {
    auto blocks = args.previous_canvas->simplify();
    int num_blocks = blocks.size();
    int canvas_size = args.previous_canvas->width * args.previous_canvas->height;
    int block_size = blocks[0]->size.px * blocks[0]->size.py;

    // 頂点番号
    auto get_node_index_source = [=]() { return 0; };
    auto get_node_index_dst_block = [=](int block_index) { return get_node_index_source() + 1 + block_index; };
    auto get_node_index_src_block = [=](int block_index) { return get_node_index_dst_block(num_blocks) + block_index; };
    auto get_node_index_sink = [=]() { return get_node_index_src_block(num_blocks); };
    int n = get_node_index_sink() + 1;

    // グラフを構築する
    graph g(n);
    constexpr const int64_t kMaxCapacity = std::numeric_limits<int>::max();

    // sourceから移動先ブロック位置へのエッジを追加する
    for (int block_index = 0; block_index < num_blocks; ++block_index) {
      g.add_edge(get_node_index_source(), get_node_index_dst_block(block_index), 1, 0);
    }

    // 移動先ブロック位置から移動元ブロック位置へのエッジを追加する
    for (int dst_block_index = 0; dst_block_index < num_blocks; ++dst_block_index) {
      for (int src_block_index = 0; src_block_index < num_blocks; ++src_block_index) {
        const auto& src_block = blocks[src_block_index];
        const auto& dst_block = blocks[dst_block_index];
        int inst_cost;
        if (src_block->id == dst_block->id) {
          // 移動しない場合はコスト0
          inst_cost = 0;
        }
        else {
          inst_cost = getCost(SwapInstruction(src_block->id, dst_block->id), block_size, canvas_size);
        }

        int sim_cost = 0;
        for (int x = dst_block->bottomLeft.px; x < dst_block->topRight.px; ++x) {
          for (int y = dst_block->bottomLeft.py; y < dst_block->topRight.py; ++y) {
            sim_cost += SimilarityChecker::pixelDiff((*args.painting)(x, y), src_block->color);
          }
        }

        g.add_edge(get_node_index_dst_block(dst_block_index), get_node_index_src_block(src_block_index), 1, inst_cost + sim_cost);
      }
    }

    // 移動元ブロック位置からsinkへのエッジを追加する
    for (int block_index = 0; block_index < num_blocks; ++block_index) {
      g.add_edge(get_node_index_src_block(block_index), get_node_index_sink(), 1, 0);
    }

    // 最小コストフローを求める
    auto [flow, cost] = g.min_cost_max_flow(get_node_index_source(), get_node_index_sink());

    // 移動元・移動先のブロックのリストを作る。
    std::vector<int> src_to_dst(num_blocks);
    for (int src_block_index = 0; src_block_index < num_blocks; ++src_block_index) {
      for (const auto& edge : g.adj[get_node_index_src_block(src_block_index)]) {
        if (edge.flow == -1) {
          src_to_dst[src_block_index] = edge.dst - get_node_index_dst_block(0);
          break;
        }
      }
    }

    // 出力を生成する。
    SolverOutputs solver_outputs;
    std::vector<bool> proceeded(num_blocks);
    for (int start_block_index = 0; start_block_index < num_blocks; ++start_block_index) {
      if (proceeded[start_block_index]) {
        continue;
      }
      proceeded[start_block_index] = true;

      int current_block_index = start_block_index;
      int next_block_index = src_to_dst[current_block_index];
      while (start_block_index != next_block_index) {
        proceeded[next_block_index] = true;
        solver_outputs.solution.push_back(std::make_shared<SwapInstruction>(blocks[current_block_index]->id, blocks[next_block_index]->id));
        std::swap(blocks[current_block_index], blocks[next_block_index]);
        current_block_index = next_block_index;
        next_block_index = src_to_dst[current_block_index];
      }
    }

    return solver_outputs;
  }
};

REGISTER_SOLVER_WITH_OPTION("MinimumCostMaximumFlow", MinimumCostMaximumFlow, MinimumCostMaximumFlow::Option);
// vim:ts=2 sw=2 sts=2 et ci
