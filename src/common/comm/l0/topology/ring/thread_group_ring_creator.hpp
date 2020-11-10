/*
 Copyright 2016-2020 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#pragma once
#include "common/comm/l0/topology/topology_construction_utils.hpp"

namespace ccl {
struct context_comm_addr;
}

namespace native {

class thread_group_ring_topology {
    thread_group_context& context;
    device_storage& devices_factory;

public:
    static constexpr ccl::group_split_type group_id() {
        return ccl::group_split_type::process;
    }

    static constexpr const char* name() {
        return "thread_group_ring_creator";
    }

    thread_group_ring_topology(thread_group_context& ctx, device_storage& devs);

    static size_t default_property_p2p_rating_calculator(const ccl_device& lhs,
                                                         const ccl_device& rhs);
    static detail::adjacency_matrix build_p2p_capability_matrix(
        std::ostream& out,
        const ccl::process_aggregated_device_mask_t& per_thread_device_masks,
        detail::p2p_rating_function ping = default_property_p2p_rating_calculator);
    static detail::adjacency_matrix build_p2p_capability_matrix(
        std::ostream& out,
        const ccl::process_device_indices_type& per_thread_device_indices,
        detail::p2p_rating_function ping = default_property_p2p_rating_calculator);

    bool build(std::ostream& out,
               const ccl::context_comm_addr& context_addr,
               const ccl::process_aggregated_device_mask_t& per_thread_device_masks,
               const detail::adjacency_matrix& matrix,
               detail::p2p_rating_function ping = default_property_p2p_rating_calculator);
    bool build(std::ostream& out,
               const ccl::context_comm_addr& context_addr,
               const ccl::process_device_indices_type& per_thread_device_indices,
               const detail::adjacency_matrix& matrix,
               detail::p2p_rating_function ping = default_property_p2p_rating_calculator);

private:
    bool build_specific(std::ostream& out,
                        const ccl::context_comm_addr& context_addr,
                        const ccl::process_device_indices_type& per_thread_device_indices,
                        const detail::plain_graph& graph);
    bool build_scale_up_specific(std::ostream& out,
                                 const ccl::context_comm_addr& context_addr,
                                 const ccl::process_device_indices_type& per_thread_device_indicess,
                                 const detail::plain_graph_list& graph_list);
};
} // namespace native
