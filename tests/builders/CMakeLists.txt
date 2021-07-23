cmake_minimum_required(VERSION 3.14)

#find_package(Boost)
#FIND_PACKAGE( Boost 1.76 COMPONENTS program_options REQUIRED )

### setup boost
#message("--- locating boost dependencies ---")
#find_package(Boost 1.71.0 EXACT COMPONENTS ${CATAPULT_BOOST_COMPONENTS} REQUIRED)
#include_directories(SYSTEM ${Boost_INCLUDE_DIR})

#add_executable(AccountRestrictionBuilderTests AccountRestrictionBuilderTests.cpp)

add_library(testsss 
  AccountRestrictionBuilderTests.cpp
  AggregateTransactionBuilderTests.cpp
  AliasBuilderTests.cpp
  HashLockBuilderTests.cpp
  KeyLinkBuilderTests.cpp
  MetadataBuilderTests.cpp
  MosaicAddressRestrictionBuilderTests.cpp
  MosaicDefinitionBuilderTests.cpp
  MosaicGlobalRestrictionBuilderTests.cpp
  MosaicSupplyChangeBuilderTests.cpp
  MultisigAccountModificationBuilderTests.cpp
  NamespaceRegistrationBuilderTests.cpp
  SecretLockBuilderTests.cpp
  SecretProofBuilderTests.cpp
  TransactionBuilderTests.cpp
  TransferBuilderTests.cpp
)


#target_link_libraries( AccountRestrictionBuilderTests
#  ${Boost_LIBRARIES}
#  builders
#  testlib
#  crypto
#  io
#  ionet
#  model
#  thread
#  utils
#  gtest_main
#)
#
