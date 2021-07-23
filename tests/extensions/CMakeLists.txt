cmake_minimum_required(VERSION 3.14)


add_library(testssss
  Bip32Tests.cpp
  Bip39Tests.cpp
  BlockExtensionsTests.cpp
  ConversionExtensionsTests.cpp
  IdGeneratorTests.cpp
  MemoryBlockStorageTests.cpp
  MemoryStreamTests.cpp
  MosaicUnitTests.cpp
  RemoteDiagnosticApiTests.cpp
  TransactionExtensionsTests.cpp
  )
