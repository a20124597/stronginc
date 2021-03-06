#include <gflags/gflags.h>
#include <glog/logging.h>
#include<boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
//#include "cpp/core/graphapi.h"
#include "cpp/serial/dualsimulation.h"
#include "cpp/serial/dual_incremental.h"
#include "cpp/serial/strongsimulation.h"
#include "cpp/serial/strong_incremental.h"
#include "cpp/utils/util.h"
#include "cpp/io/io_local.h"
#include "cpp/core/global.h"
#include "cpp/core/strongr.h"
#include "cpp/core/view.h"
#include "cpp/utils/generate.h"
#include<iostream>
#include <fstream>
#include<ctime>
#include <sys/time.h>
#include<boost/filesystem.hpp>
#define random(a,b) (rand()%(b-a+1)+a)


class Serial{
public:
   double get_current_time() {
	timeval t;
	gettimeofday(&t, 0);
	return (double) t.tv_sec + (double) t.tv_usec / 1000000;
    }

    Serial(){}

    Serial(std::string test_data_name,int query_index){
        this->query_index = query_index;
        this->test_data_name=test_data_name;
        this->graph_vfile ="../data/"+test_data_name+"/"+test_data_name+".v";
        this->graph_efile ="../data/"+test_data_name+"/"+test_data_name+".e";
        this->view_file = "../data/"+test_data_name+"/views/view";
        this->r_file = "../data/"+test_data_name+"/"+test_data_name+".r";
        this->base_qfile = "../data/"+test_data_name+"/query/q";
        this->base_add_file = "../data/"+test_data_name+"/inc/add_e";
        this->base_remove_file="../data/"+test_data_name+"/inc/rm_e";
    }

    std::string get_query_vfile(int index){
        return base_qfile+std::to_string(index)+".v";
    }

    std::string get_query_efile(int index){
        return base_qfile+std::to_string(index)+".e";
    }

    std::string get_view_vfile(int query_index,int view_index){
        return view_file+std::to_string(query_index)+"/view"+std::to_string(view_index)+".v";
    }
    std::string get_view_efile(int query_index,int view_index){
       return view_file+std::to_string(query_index)+"/view"+std::to_string(view_index)+".e";
    }

    bool is_exist_file(std::string file_name){
        return boost::filesystem::is_directory(file_name);
    }

    void make_dir(std::string file_name){
        if(is_exist_file(file_name)){
             return ;
        }
        boost::filesystem::create_directory(file_name);
    }
public:

  void generate_query(int generate_query_nums,int generate_query_nodes, int max_calculate_center_nodes){
      Graph dgraph;
      Generate generate;
      GraphLoader dgraph_loader;
      dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
      std::cout<<dgraph.GetNumVertices()<<' '<<dgraph.GetNumEdges()<<std::endl;
      int i=1;
      while(i<=generate_query_nums){
          Graph qgraph;
          generate.generate_connect_graphs_by_Dgraph(dgraph,qgraph,generate_query_nodes);
          int d_Q=cal_diameter_qgraph(qgraph);
          if(d_Q>2 || !query_labl_all_notsame(qgraph)){
              continue;
          }
          clock_t s0,e0;
          s0 =clock();
          std::unordered_set<VertexID> max_dual_set = generate.get_dual_node_result(dgraph,qgraph);
          e0 =clock();
          if(max_dual_set.size()<=max_calculate_center_nodes){
              generate.save_grape_file(qgraph,get_query_vfile(i),get_query_efile(i));
              std::cout<<i<<' '<<"calculate dual time"<<(float)(e0-s0)/CLOCKS_PER_SEC<<"s"<<' '<<max_dual_set.size()<<std::endl;
              i++;
          }
      }
  }

  void test_generate_query(int generate_query_nums,int generate_query_nodes, int generate_query_edges, int max_calculate_center_nodes){
      Graph dgraph;
      Generate generate;
      GraphLoader dgraph_loader;
      dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
      std::cout<<dgraph.GetNumVertices()<<' '<<dgraph.GetNumEdges()<<std::endl;
      int i=1;
      while(i<=generate_query_nums){
          Graph qgraph;
          //generate.generate_connect_graphs_by_Dgraph(dgraph,qgraph,generate_query_nodes);
          generate.generate_view_by_Qgraph(dgraph,qgraph,generate_query_nodes);
          int d_Q=cal_diameter_qgraph(qgraph);
          if(d_Q!=4 || qgraph.GetNumEdges()!=generate_query_edges){
              continue;
          }
          double s0=get_current_time();
          std::unordered_set<VertexID> max_dual_set = generate.get_dual_node_result(dgraph,qgraph);
          double e0=get_current_time();

          if(max_dual_set.size()<=max_calculate_center_nodes){
              generate.save_grape_file(qgraph,get_query_vfile(i),get_query_efile(i));
              std::cout<<i<<" query nodes: "<<qgraph.GetNumVertices()<<' '<<" query edges: "<<qgraph.GetNumEdges()<<" "<<"d_Q: "<<d_Q<<" max_dual_set: "<<max_dual_set.size()<<" times: "<<e0-s0<<std::endl;
              i++;
          }
      }
  }

  void test_print_graph_label_set(){
      Graph dgraph;
      GraphLoader dgraph_loader;
      dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
      std::cout<<dgraph.GetNumVertices()<<' '<<dgraph.GetNumEdges()<<std::endl;
      std::unordered_set<VertexLabel> label_set;
      for(auto u :dgraph.GetAllVerticesID()){
           label_set.insert(dgraph.GetVertexLabel(u));
      }
      for(int i=0;i<label_set.size();++i){
          if(label_set.find(i)==label_set.end()){
             cout<<i<<endl;
          }
      }
      cout<<label_set.size()<<endl;
  }

  void test_generate_random_connect_graph(int num_nodes,int num_edges,int labels,int generate_query_nums){
      Generate generate;
      int i=1;
      while(i<=generate_query_nums){
           Graph qgraph;
           generate.generate_random_connectivity_graph(qgraph,num_nodes,num_edges,labels);
           std::unordered_set<VertexID> node_set;
           qgraph.find_connectivity_nodes(0,node_set);
           cout<<qgraph.GetNumVertices()<<' '<<qgraph.GetNumEdges()<<' '<<(node_set.size()==num_nodes)<<endl;
           /*
           for(auto u:qgraph.GetAllVerticesID()){
               cout<<u<<' '<<qgraph.GetVertexLabel(u)<<endl;
           }
           for(auto e:qgraph.GetAllEdges()){
               cout<<e.src()<<' '<<e.dst()<<' '<<e.attr()<<endl;
           }
           */
           i++;
      }
  }

    void generate_random_dgraph(int num_nodes=2000,double a = 1.20,int l = 10){
        Graph dgraph;
        Generate generate;
        generate.generate_random_dgraph(dgraph,num_nodes,a,l);
        generate.save_grape_file(dgraph,graph_vfile,graph_efile);
        Graph dgraph1;
        GraphLoader dgraph_loader;
        dgraph_loader.LoadGraph(dgraph1,graph_vfile,graph_efile);
        cout<<dgraph1.GetNumVertices()<<' '<<dgraph1.GetNumEdges()<<endl;
    }

    void test_add_edges(){
        int index = 1;
        GraphLoader dgraph_loader,qgraph_loader;
        Graph qgraph;
        qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
//        cout<<qgraph.GetNumEdges()<<endl;
//        for(auto e:qgraph.GetAllEdges()){
//            std::cout<<e.src()<<' '<<e.dst()<<endl;
//        }
//        qgraph.AddEdge(Edge(1,2,33));
//        cout<<qgraph.GetNumEdges()<<endl;
//        for(auto e:qgraph.GetAllEdges()){
//            std::cout<<e.src()<<' '<<e.dst()<<endl;
//        }
//        qgraph.AddEdge(Edge(0,2,44));
//        cout<<qgraph.GetNumEdges()<<endl;
//        for(auto e:qgraph.GetAllEdges()){
//            std::cout<<e.src()<<' '<<e.dst()<<endl;
//        }
//        qgraph.AddEdge(Edge(0,1,55));
//        cout<<qgraph.GetNumEdges()<<endl;
//        for(auto e:qgraph.GetAllEdges()){
//            std::cout<<e.src()<<' '<<e.dst()<<endl;
//        }
        std::unordered_set<Edge> edge_set;
        for(auto e:qgraph.GetAllEdges()){
            edge_set.insert(e);
        }
        std::cout<<edge_set.size()<<endl;
        edge_set.insert(Edge(0,1,44));
        std::cout<<edge_set.size()<<' '<<(edge_set.find(Edge(0,1,55))!=edge_set.end())<<std::endl;
        for(auto e:edge_set){
            std::cout<<e.src()<<' '<<e.dst()<<' '<<e.attr()<<std::endl;
        }

        edge_set.insert(Edge(0,2,55));
        std::cout<<edge_set.size()<<' '<<(edge_set.find(Edge(0,1,55))!=edge_set.end())<<std::endl;
        for(auto e:edge_set){
            std::cout<<e.src()<<' '<<e.dst()<<' '<<e.attr()<<std::endl;
        }

        edge_set.insert(Edge(1,2,88));
        std::cout<<edge_set.size()<<' '<<(edge_set.find(Edge(0,1,55))!=edge_set.end())<<std::endl;
        for(auto e:edge_set){
            std::cout<<e.src()<<' '<<e.dst()<<' '<<e.attr()<<std::endl;
        }
    }

    void test_dualsimulation(){
        Graph dgraph;
        GraphLoader dgraph_loader,qgraph_loader;
        dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
        DualSim dualsim;
        int index = 1;
        while(index<2){
            Graph qgraph;
            qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
            std::unordered_map<VertexID, std::unordered_set<VertexID>>  sim;
            bool initialized_sim = false;
            dualsim.dual_simulation(dgraph,qgraph,sim,initialized_sim);
            for(auto u :qgraph.GetAllVerticesID()){
                cout<<sim[u].size()<<endl;
            }
            save_sim_result(qgraph,sim,"../data/dualresult"+std::to_string(index));
            index+=1;
        }
    }

    void test_dual_incremental(){
        DualInc dualinc;
        DualSim dualsim;
        Graph dgraph;
        GraphLoader dgraph_loader,qgraph_loader;
        dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
        int index =1;
        while(index <200){
            Graph qgraph;
            qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
            std::unordered_map<VertexID, std::unordered_set<VertexID>> sim;
            bool initialized_sim=false;
            dualsim.dual_simulation(dgraph,qgraph,sim,initialized_sim);
            int j=1;
            while (j<20){
            std::set<std::pair<VertexID,VertexID>> add_edges,rm_edges;
            Load_bunch_edges(add_edges,base_add_file,j);
            Load_bunch_edges(rm_edges,base_remove_file,j);
            for (auto e:add_edges){
               dgraph.AddEdge(Edge(e.first,e.second,1));
            }
            std::unordered_map<VertexID, std::unordered_set<VertexID>> incdsim,direct_sim;
            for(auto u :qgraph.GetAllVerticesID()){
                incdsim[u]=std::unordered_set<VertexID>();
                for(auto v:sim[u]){
                incdsim[u].insert(v);
                }
            }
            dualsim.dual_simulation(dgraph,qgraph,direct_sim,initialized_sim);
            for (auto e:add_edges){
               dgraph.AddEdge(Edge(e.first,e.second,1));
            }
            dualinc.incremental_addedges(dgraph,qgraph,incdsim,add_edges);
            std::cout<<index<<' '<<j<<' '<<dual_the_same(qgraph,direct_sim,incdsim)<<std::endl;
            for(auto e :rm_edges){
                dgraph.RemoveEdge(Edge(e.first,e.second,1));
            }
            direct_sim.clear();
            dualsim.dual_simulation(dgraph,qgraph,direct_sim,initialized_sim);
            dualinc.incremental_removeedgs(dgraph,qgraph,incdsim,rm_edges);
            std::cout<<index<<' '<<j<<' '<<dual_the_same(qgraph,direct_sim,incdsim)<<std::endl;
            for(auto e:rm_edges){
                dgraph.AddEdge(Edge(e.first,e.second,1));
            }
            for (auto e:add_edges){
               dgraph.RemoveEdge(Edge(e.first,e.second,1));
            }
            j+=1;
           }
           index +=1;
        }

    }

    void test_strongsimulation(){
    StrongSim strongs;
    Graph dgraph;
    GraphLoader dgraph_loader,qgraph_loader;
    dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
    int index = 1;
    while (index <20){
      Graph qgraph;
      qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
      clock_t start,end;
      start =clock();
      strongs.strong_simulation_sim(dgraph,qgraph);
      end = clock();
//      std::cout<<"strong simulation time "<<(float)(end-start)/CLOCKS_PER_SEC<<"s"<<std::endl;
      index +=1;
   }
 }

    void test_view_minimum_contain(){
        int index=1;
        while(index<200){
            Graph qgraph;
            //View vie;
            GraphLoader qgraph_loader;
            qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
            View vie;
            for(int i=1;i<6;++i){
                GraphLoader view_loader;
                Graph* new_view = new Graph();
                view_loader.LoadGraph(*new_view,get_view_vfile(index,i),get_view_efile(index,i));
                vie.add_ViewGraph(new_view);
            }
            vie.containCheck(qgraph);
            std::vector<int> result=vie.minimumContain(qgraph);
            std::cout<<index<<" minimum contain: ";
            for(auto num:result){
               std::cout<<num<<' ';
            }
            std::cout<<endl;
//            vie.traverse_ViewGraph();
            index+=1;
        }
}

    void test_view_minimal_contain(){
        int index=1;
        while(index<200){
            Graph qgraph;
            //View vie;
            GraphLoader qgraph_loader;
            qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
            View vie;
            for(int i=1;i<6;++i){
                GraphLoader view_loader;
                Graph* new_view = new Graph();
                view_loader.LoadGraph(*new_view,get_view_vfile(index,i),get_view_efile(index,i));
                vie.add_ViewGraph(new_view);
            }
            vie.containCheck(qgraph);
            std::vector<int> result=vie.minimalContain(qgraph);
            std::cout<<index<<" minimal contain: ";
            for(auto num:result){
               std::cout<<num<<' ';
            }
            std::cout<<endl;
//            vie.traverse_ViewGraph();
            index+=1;
        }
}

    void generate_query_view(int generate_view_nodes){
        int index = 1;
        Generate generate;
        while(index<=199){
            Graph qgraph;
            View vie;
            GraphLoader qgraph_loader;
           // cout<<is_exist_file(view_file+std::to_string(index));
            string tmp_path = view_file+std::to_string(index);
            if(!is_exist_file(tmp_path)){
                 //cout<<"file does not exist,prepare to mkdir"<<endl;
                 make_dir(tmp_path);
            }
            qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
            for(int i=1;i<6;++i){
                GraphLoader view_loader;
                Graph* new_view = new Graph();
                generate.generate_view_by_Qgraph(qgraph,*new_view,generate_view_nodes);
               // generate.generate_connect_graphs_by_Dgraph(qgraph,*new_view,generate_view_nodes);
                vie.add_ViewGraph(new_view);
            }

            bool is_contain = vie.containCheck(qgraph);
            if(is_contain){
                std::vector<int> result=vie.minimumContain(qgraph);
//                if(result.size()==3){
//                    continue;
//                }
                cout<<index<<' '<<"minicontain nums ";
                for(auto num:result){
                   std::cout<<num<<' ';
                }
                std::cout<<endl;
                std::vector<Graph*> tmp_view_list= vie.get_ViewGraph_list();
                for(int i=0;i<tmp_view_list.size();++i){
                    generate.save_grape_file(*tmp_view_list[i],get_view_vfile(index,i+1),get_view_efile(index,i+1));
                }
                index++;
            }
    }
}

    vector<float> compare_direct_and_view_strongresult(Graph &qgraph, std::vector<StrongR> &direct_strong_result,std::vector<StrongR> &view_strong_result){
        vector<float> result;
//        if(direct_strong_result.size()!=view_strong_result.size()){
//            cout<<"size not the same "<<direct_strong_result.size()<<' '<<view_strong_result.size()<<endl;
//            return result;
//        }
//        for(int i=0;i<direct_strong_result.size();++i){
//            if(direct_strong_result[i].center() != view_strong_result[i].center()){
//                std::cerr<<"have different center result"<<endl;
//                return result;
//            }
//        }
        for(int i=0;i<direct_strong_result.size();++i){
            bool find=false;
            for(int j=0;j<view_strong_result.size();++j){
                if(direct_strong_result[i].center() == view_strong_result[j].center()){
                   std::unordered_map<VertexID, std::unordered_set<VertexID>> dirctsim=direct_strong_result[i].ballr();
                   std::unordered_map<VertexID, std::unordered_set<VertexID>> viewsim=view_strong_result[j].ballr();
                   if(dual_the_same(qgraph,dirctsim,viewsim)){
                      result.push_back(1.0);
                   }else{
                       std::unordered_set<VertexID> direct_result_node;
                       std::unordered_set<VertexID> view_result_node;
                       for(auto u:qgraph.GetAllVerticesID()){
                          for(auto v:dirctsim[u]){
                             direct_result_node.insert(v);
                          }
                          for(auto v:viewsim[u]){
                              view_result_node.insert(v);
                           }
                       }
                       result.push_back(float(view_result_node.size()*1.0/direct_result_node.size()));
                }
                find=true;
                break;
            }
        }
        if(!find){
            //result.push_back(0.0);
        }

        }
        return result;
    }

    int get_dual_result_num(Graph &dgraph,Graph &qgraph){
        std::unordered_set<VertexID> result;
        DualSim dualsim;
        std::unordered_map<VertexID, std::unordered_set<VertexID>>  sim;
        bool initialized_sim = false;
        dualsim.dual_simulation(dgraph,qgraph,sim,initialized_sim);
        for(auto u:qgraph.GetAllVerticesID()){
            for(auto v:sim[u]){
                result.insert(v);
            }
        }
        return result.size();
    }

    void print_view_info(int circle_num,std::string view_info_text="view_result_number.txt"){
        Graph dgraph;
        GraphLoader dgraph_loader,qgraph_loader;
        dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
        cout<<dgraph.GetNumVertices()<<' '<<dgraph.GetNumEdges()<<endl;

        int index=1;
        std::fstream outfile(view_info_text,std::ios::out);
        while(index<=circle_num){
            Graph qgraph;
            qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
            std::fstream outfile(view_info_text,std::ios::app);
            int n=get_dual_result_num(dgraph,qgraph);
            outfile<<n;
            View vie;
            for(int i=1;i<6;++i){
                GraphLoader view_loader;
                Graph* new_view = new Graph();
                view_loader.LoadGraph(*new_view,get_view_vfile(index,i),get_view_efile(index,i));
                vie.add_ViewGraph(new_view);
                n=get_dual_result_num(dgraph,*new_view);
                outfile<<'\t'<<n;
            }
            outfile<<endl;
            outfile.close();
        std::cout<<"query view combination: "<<index<<endl;
        index+=1;
        }
    }

    void test_view_query_all(int circle_num,int flag0,int flag1){//flag0=0 means do not cache node distance,//flag1==0 means minimum contain,flag1=1 means minimalcontain
        Graph dgraph;
        GraphLoader dgraph_loader,qgraph_loader;
        dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
        cout<<dgraph.GetNumVertices()<<' '<<dgraph.GetNumEdges()<<endl;
        int index=1;
        while(index<=circle_num){
        Graph qgraph;
        qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
        View vie;
        for(int i=1;i<6;++i){
                GraphLoader view_loader;
                Graph* new_view = new Graph();
                view_loader.LoadGraph(*new_view,get_view_vfile(index,i),get_view_efile(index,i));
                vie.add_ViewGraph(new_view);
        }
        double view_stime=get_current_time();
        std::vector<StrongR> view_strong_result = vie.queryByViews(dgraph,qgraph,flag0,flag1);//0 means answer using cache dual_simulation ,1 means answer using cache strong simulaton
        double view_etime=get_current_time();
        std::cerr<<"calculate view strong"<<view_etime-view_stime<<"s"<<std::endl;

        StrongSim strongs;
        double direct_stime=get_current_time();
//        vector<StrongR> direct_strong_result;
        vector<StrongR> direct_strong_result= strongs.strong_simulation_sim(dgraph,qgraph);
        double direct_etime=get_current_time();
        std::cout<<"calculate direct strong"<<direct_etime-direct_stime<<"s"<<std::endl;
       // cout<<view_strong_result.size()<<' '<<direct_strong_result.size()<<endl;
        vector<float> compare_rate = compare_direct_and_view_strongresult(qgraph,direct_strong_result,view_strong_result);
        int same_count=0;
        int appoximate_count=0;
        int center_none_result=direct_strong_result.size()-view_strong_result.size();
        for(auto f:compare_rate){
               if(f==1){
                   same_count+=1;
               }else{
                   appoximate_count+=1;
               }
       }
       cout<<index<<" same_count: "<<same_count<<" appoxiamte_count : "<<appoximate_count<<" none result count: "<<center_none_result<<" accuracy rate : "<<(float)same_count*1.0/(same_count+appoximate_count+center_none_result)<<endl;
       index+=1;
       }
    }

    void test_view_query_one(int query_index,int flag0,int flag1){//flag0=0 means do not cache node distance,//flag1==0 means minimum contain,flag1=1 means minimalcontain
        Graph dgraph;
        GraphLoader dgraph_loader,qgraph_loader;
        dgraph_loader.LoadGraph(dgraph,graph_vfile,graph_efile);
        cout<<dgraph.GetNumVertices()<<' '<<dgraph.GetNumEdges()<<endl;
        int index=query_index;
        //while(index<=circle_num){
        Graph qgraph;
        qgraph_loader.LoadGraph(qgraph,get_query_vfile(index),get_query_efile(index));
        View vie;
        for(int i=1;i<6;++i){
                GraphLoader view_loader;
                Graph* new_view = new Graph();
                view_loader.LoadGraph(*new_view,get_view_vfile(index,i),get_view_efile(index,i));
                vie.add_ViewGraph(new_view);
        }
        double view_stime=get_current_time();
        std::vector<StrongR> view_strong_result = vie.queryByViews(dgraph,qgraph,flag0,flag1);//0 means answer using cache dual_simulation ,1 means answer using cache strong simulaton
        double view_etime=get_current_time();
        std::cout<<"calculate view strong"<<view_etime-view_stime<<"s"<<std::endl;

        StrongSim strongs;
        double direct_stime=get_current_time();
//        vector<StrongR> direct_strong_result;
        vector<StrongR> direct_strong_result= strongs.strong_simulation_sim(dgraph,qgraph);
        double direct_etime=get_current_time();
        std::cout<<"calculate direct strong"<<direct_etime-direct_stime<<"s"<<std::endl;
       // cout<<view_strong_result.size()<<' '<<direct_strong_result.size()<<endl;
        vector<float> compare_rate = compare_direct_and_view_strongresult(qgraph,direct_strong_result,view_strong_result);
        int same_count=0;
        int appoximate_count=0;
        int center_none_result=direct_strong_result.size()-view_strong_result.size();
        for(auto f:compare_rate){
               if(f==1){
                   same_count+=1;
               }else{
                   appoximate_count+=1;
               }
       }
       cout<<index<<" same_count: "<<same_count<<" appoxiamte_count : "<<appoximate_count<<" none result count: "<<center_none_result<<" accuracy rate : "<<(float)same_count*1.0/(same_count+appoximate_count+center_none_result)<<endl;
      // index+=1;
      // }
    }
private:
    int query_index = 1;
    std::string test_data_name ="yago";
    std::string graph_vfile ="../data/yago/yago.v";
    std::string graph_efile ="../data/yago/yago.e";
    std::string view_file = "../data/yago/views/view";
    std::string r_file = "../data/yago/yago.r";
    std::string base_qfile = "../data/yago/query/q";
    std::string base_add_file = "../data/yago/inc/add_e";
    std::string base_remove_file="../data/yago/inc/rm_e";
};

int main(int argc, char *argv[]) {
  google::SetUsageMessage("Usage: test [gflags_opt]");
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::ShutDownCommandLineFlags();
  google::InitGoogleLogging("test for working");
  google::ShutdownGoogleLogging();
//  init_workers();
 // Serial serial("dbpedia",1);
 // serial.test_generate_query(200,5,7,1000);
  Serial serial("yago",3);
//  serial.test_generate_random_connect_graph(5,10,5,10);
  //serial.generate_random_dgraph(100,1.20,5);
  //serial.generate_query(200,5,40);
//  serial.test_dualsimulation();
//  serial.test_dual_incremental();
//  serial.test_strongsimulation();
//  serial.test_add_edges();


    serial.test_view_minimum_contain();
    //serial.generate_query_view(3);
  //  serial.test_view_query_one(49,1,0);
//   serial.print_view_info(199);
    //serial.test_generate_query(200,6,12,2000);
  //serial.test_view_query_all(49,1,0);
    //serial.test_view_query();
//  worker_finalize();
  return 0;
}

using namespace std;