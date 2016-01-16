# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# All rights reserved.
# See file CREDITS.txt for details.

# Declare micro-libs:
add_library(quickstep_queryoptimizer_costmodel_CostModel ../../empty_src.cpp CostModel.hpp)
add_library(quickstep_queryoptimizer_costmodel_SimpleCostModel SimpleCostModel.cpp SimpleCostModel.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_costmodel_SimpleCostModel
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_costmodel ../../empty_src.cpp CostModelModule.hpp)
target_link_libraries(quickstep_queryoptimizer_costmodel
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_costmodel_SimpleCostModel)
