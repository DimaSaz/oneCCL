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

#include "common/comm/l0/communicator/base_communicator.hpp"

namespace native
{
    template<ccl::device_topology_type schema_id>
    struct device_community;
}

template<class comm_impl, ccl::device_topology_type topology, class communicator_traits>
class typed_base_communicator : public base_communicator
{
public:
    using base_t = base_communicator;
    using impl_t = comm_impl;
    using self_t = typed_base_communicator<comm_impl, topology, communicator_traits>;
    using traits = communicator_traits;

    // Topologies
    static constexpr ccl::device_topology_type topology_type()
    {
        return topology;
    }

    static constexpr ccl::device_topology_class topology_class()
    {
        return ccl::topology_to_class<topology>();
    }

    // traits
    bool is_host() const noexcept override
    {
        return traits::is_host();
    }

    bool is_cpu() const noexcept override
    {
        return traits::is_cpu();
    }

    bool is_gpu() const noexcept override
    {
        return traits::is_gpu();
    }

    bool is_accelerator() const noexcept override
    {
        return traits::is_accelerator();
    }

    typed_base_communicator(ccl::unified_device_type&& device,
                            size_t thread_idx, size_t process_idx,
                            const ccl::device_comm_attr_t& attr);

    ccl::device_topology_type get_topology_type() const override;

    void initialize_comm_addr(const ccl::device_index_type& device_id,
                              std::shared_ptr<native::device_community<topology>> community);

    bool is_ready() const override;

    // communicator interfaces implementation
    COMM_INTERFACE_COLL_DEFINITION__VOID;
    COMM_INTERFACE_COLL_DEFINITION(char);
    COMM_INTERFACE_COLL_DEFINITION(int);
    COMM_INTERFACE_COLL_DEFINITION(int64_t);
    COMM_INTERFACE_COLL_DEFINITION(uint64_t);
    COMM_INTERFACE_COLL_DEFINITION(float);
    COMM_INTERFACE_COLL_DEFINITION(double);

#ifdef CCL_ENABLE_SYCL
    COMM_INTERFACE_COLL_CLASS_DEFINITION(cl::sycl::buffer<char COMMA 1>);
    COMM_INTERFACE_COLL_CLASS_DEFINITION(cl::sycl::buffer<int COMMA 1>);
    COMM_INTERFACE_COLL_CLASS_DEFINITION(cl::sycl::buffer<int64_t COMMA 1>);
    COMM_INTERFACE_COLL_CLASS_DEFINITION(cl::sycl::buffer<uint64_t COMMA 1>);
    COMM_INTERFACE_COLL_CLASS_DEFINITION(cl::sycl::buffer<float COMMA 1>);
    COMM_INTERFACE_COLL_CLASS_DEFINITION(cl::sycl::buffer<double COMMA 1>);
#endif //CCL_ENABLE_SYCL

    COMM_INTERFACE_SPARSE_DEFINITION__VOID;
    COMM_INTERFACE_SPARSE_DEFINITION(char, char);
    COMM_INTERFACE_SPARSE_DEFINITION(char, int);
    COMM_INTERFACE_SPARSE_DEFINITION(char, ccl::bfp16);
    COMM_INTERFACE_SPARSE_DEFINITION(char, float);
    COMM_INTERFACE_SPARSE_DEFINITION(char, double);
    COMM_INTERFACE_SPARSE_DEFINITION(char, int64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(char, uint64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(int, char);
    COMM_INTERFACE_SPARSE_DEFINITION(int, int);
    COMM_INTERFACE_SPARSE_DEFINITION(int, ccl::bfp16);
    COMM_INTERFACE_SPARSE_DEFINITION(int, float);
    COMM_INTERFACE_SPARSE_DEFINITION(int, double);
    COMM_INTERFACE_SPARSE_DEFINITION(int, int64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(int, uint64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, char);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, int);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, ccl::bfp16);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, float);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, double);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, int64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(int64_t, uint64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, char);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, int);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, ccl::bfp16);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, float);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, double);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, int64_t);
    COMM_INTERFACE_SPARSE_DEFINITION(uint64_t, uint64_t);

#ifdef CCL_ENABLE_SYCL
    COMM_INTERFACE_SPARSE_CLASS_DEFINITION(cl::sycl::buffer<int COMMA 1>,
                                           cl::sycl::buffer<float COMMA 1>);
    COMM_INTERFACE_SPARSE_CLASS_DEFINITION(cl::sycl::buffer<int COMMA 1>,
                                           cl::sycl::buffer<ccl::bfp16 COMMA 1>);

    COMM_INTERFACE_SPARSE_CLASS_DEFINITION(cl::sycl::buffer<int64_t COMMA 1>,
                                           cl::sycl::buffer<float COMMA 1>);
    COMM_INTERFACE_SPARSE_CLASS_DEFINITION(cl::sycl::buffer<int64_t COMMA 1>,
                                           cl::sycl::buffer<ccl::bfp16 COMMA 1>);
#endif //CCL_ENABLE_SYCL

    // Device community interface
    template<class device_t>
    size_t get_device_count() const;

    template<class device_t>
    native::indexed_device_container<device_t>& get_devices();

    // troubleshooting
    std::string to_string() const;

    std::shared_ptr<native::device_community<topology>> device_community_impl;

    impl_t* get_impl()
    {
        return static_cast<impl_t*>(this);
    }
};
