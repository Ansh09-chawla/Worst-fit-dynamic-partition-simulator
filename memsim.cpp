
#include "memsim.h"
#include <cassert>
#include <iterator>
#include <iostream>
#include <set>
#include <unordered_map>
#include <list>
#include <math.h>

struct Partition {
int tag = -1;
int64_t size =0, addr = 0;
};

typedef std::list<Partition>::iterator PartitionRef;
typedef std::set<PartitionRef>::iterator TreeRef;

struct scmp {
  bool operator()(const PartitionRef & c1, const PartitionRef & c2) const {
    if (c1->size == c2->size)
      return c1->addr < c2->addr;
    else
      return c1->size > c2->size;
  }
};

struct Simulator{

  int64_t page_size = 0;

  std::list<Partition> all_blocks;
  
  std::set<PartitionRef,scmp> free_blocks;

  std::unordered_map<long, std::vector<PartitionRef>> tagged_blocks;

  MemSimResult result;

  Simulator(int64_t page_size){


    this->page_size = page_size;
    result.n_pages_requested = 0;

  }


  void allocate(int tag, int size){
    if(all_blocks.empty()){
      Partition first_partition;
      first_partition.size = (ceil((double)size/(double)page_size))*page_size;
      this->result.n_pages_requested += ceil((double)size/(double)page_size);
      all_blocks.push_back(first_partition);
      free_blocks.insert(all_blocks.begin());
    }
    PartitionRef largest_empty_partition;
    bool no_worst_fit_found = true;
    if(!free_blocks.empty()){
      auto tree_iterator = free_blocks.begin();
      if((*tree_iterator)->size>=size){
        largest_empty_partition = *tree_iterator;
        no_worst_fit_found = false;
      }  
    }
    if(no_worst_fit_found){   
      
      if((std::prev(all_blocks.end())->tag)!=-1){ 
        Partition partition_to_be_added;
        this->result.n_pages_requested += ceil((double)size/(double)page_size);
        partition_to_be_added.size = (ceil((double)size/(double)page_size))*page_size;
        partition_to_be_added.addr = std::prev(all_blocks.end())->addr +  std::prev(all_blocks.end())->size;
        all_blocks.push_back(partition_to_be_added);
      }else{

        this->result.n_pages_requested += (ceil(((double)size-(double)std::prev(all_blocks.end())->size)/((double)page_size)));
        TreeRef tree_it = free_blocks.find(std::prev(all_blocks.end()));
        free_blocks.erase(tree_it);
        std::prev(all_blocks.end())->size += (ceil(((double)size-(double)std::prev(all_blocks.end())->size)/((double)page_size)))*page_size;
        
      } 
      largest_empty_partition  = std::prev(all_blocks.end()); 
      free_blocks.insert(std::prev(all_blocks.end()));
    }
  
    if(largest_empty_partition->size > size){
      TreeRef tree_it = free_blocks.find(largest_empty_partition);
      free_blocks.erase(tree_it);
      int size_of_splitted = largest_empty_partition->size - size;
      largest_empty_partition->tag = tag;
      largest_empty_partition->size = size;
      tagged_blocks[tag].push_back(largest_empty_partition);
      Partition partition_to_be_added;
      partition_to_be_added.size = size_of_splitted;
      partition_to_be_added.addr = largest_empty_partition-> addr + largest_empty_partition-> size;

      all_blocks.insert(std::next(largest_empty_partition), partition_to_be_added);
      free_blocks.insert(std::next(largest_empty_partition));  
      
    }else{
      TreeRef tree_it = free_blocks.find(largest_empty_partition);
      free_blocks.erase(tree_it);
      largest_empty_partition->tag = tag;
      tagged_blocks[tag].push_back(largest_empty_partition);
    }
  }
  void deallocate(int tag)
  {
    std::vector<PartitionRef> partition_with_tag = tagged_blocks[tag];
    PartitionRef current;
    
    for(int i = 0; i< (int)partition_with_tag.size();i++){
      current = partition_with_tag[i];
      current->tag = -1;  
      if(current!=all_blocks.begin() && std::prev(current)->tag==-1){
        TreeRef tree_it = free_blocks.find(std::prev(current));
        free_blocks.erase(tree_it);
        current->size+=std::prev(current)->size;
        current->addr = std::prev(current)->addr;
        all_blocks.erase(std::prev(current));
      }
      if(current!=std::prev(all_blocks.end()) &&  std::next(current)->tag==-1){
        TreeRef tree_it = free_blocks.find(std::next(current));
        free_blocks.erase(tree_it);
        current->size+= std::next(current)->size;
        all_blocks.erase(std::next(current));
      }
      free_blocks.insert(current);
    }
    tagged_blocks.erase(tag);

  }  
  MemSimResult getStats()
  {
    // let's guess the result... :)

    this->result.max_free_partition_size = 0;
    this->result.max_free_partition_address = 0;
    PartitionRef it;
    for (it = all_blocks.begin(); it != all_blocks.end(); ++it) {
        if(it->size > this->result.max_free_partition_size && it->tag==-1){
          this->result.max_free_partition_size = it->size;
          this->result.max_free_partition_address = it->addr;
        }  
    }
    return this->result;
  }
} ;

MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests)
{

  Simulator sim(page_size);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }

  }
  return sim.getStats();
}
