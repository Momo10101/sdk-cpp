/**
*** Copyright (c) 2016-2019, Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp.
*** Copyright (c) 2020-present, Jaguar0625, gimre, BloodyRookie.
*** All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#include "symbol/core/model/Address.h"
#include "symbol/core/model/NetworkIdentifier.h"
#include "symbol/core/utils/HexFormatter.h"
#include "tests/shared/nodeps/KeyTestUtils.h"
#include "tests/shared/nodeps/TestNetworkConstants.h"
#include "tests/TestHarness.h"

namespace catapult {

#define TEST_CLASS AddressIntegrityTests

	TEST(TEST_CLASS, CanFindAddressStartingWithPA) {
		// Arrange:
		for (auto i = 0u; i < 1000; ++i) {
			auto kp = test::GenerateKeyPair();
			auto rawAddress = model::PublicKeyToAddress(kp.publicKey(), model::NetworkIdentifier::Private);
			auto address = model::AddressToString(rawAddress);
			if (address[0] == 'P' && address[1] == 'A')
				return;
		}

		FAIL() << "could not generate PA* address";
	}

	namespace {
		struct PrivateTestNetworkTraits {
			static constexpr auto Network_Identifier = model::NetworkIdentifier::Private_Test;
			static constexpr auto Nemesis_Private_Key = test::Test_Network_Nemesis_Private_Key;
			static constexpr auto Expected_Nemesis_Address = "VARNAMOK7M4ZHNL75ZNNYC6UAM5EGH57UGHVZIA";

			static std::vector<const char*> PrivateKeys() {
				return std::vector<const char*>(&test::Test_Network_Private_Keys[0], &test::Test_Network_Private_Keys[11]);
			}

			static std::vector<std::string> ExpectedAddresses() {
				return {
					"VAAA26Z5QXYHDIVO2EYSEF7BUMC7FYQPBSWEMGQ",
					"VAAA3ZE57CKRZY762QG4OG3RMM2EEAN6SIGNQ7Y",
					"VAAA4RUOJ2RBEKWYOZHSMZWTDGHMCRDUCPOEXKY",
					"VAAA6NI72BJ4FEE24JNVTSQRZHXGZCZCJ7BKOBQ",
					"VAAA7O53WQULXNOK2GTP7QURX7RKPRDNF4SBYXQ",
					"VAAAC7XL2E6XC2NCSXI4C6MG7HWZGE47WEVZQOA",
					"VAAADPWJ27OZVPSPMWCX5QQWWC47IF6KUFEWPEQ",
					"VAAAEBW5QTJAYLWOYUZDJO7BJIZCIFRP3FWJKAY",
					"VAAAFAXT2EHGLAOXOR6I3JXR27ED7Q3LEFUDUYA",
					"VAAAG6WJ6IHREIOJN4U5H6GV5HAQLWU7SMXJYSY",
					"VAAAHTKTAW4NLGLXU7U2ZWP5LWQ4Y7DGYVDIIJA"
				};
			}
		};

		auto PrivateKeyStringToAddressString(const std::string& pkString, model::NetworkIdentifier networkIdentifier) {
			auto kp = crypto::KeyPair::FromString(pkString);
			auto rawAddress = model::PublicKeyToAddress(kp.publicKey(), networkIdentifier);
			return model::AddressToString(rawAddress);
		}
	}

#define TRAITS_BASED_TEST(TEST_NAME) \
	template<typename TTraits> void TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)(); \
	TEST(TEST_CLASS, TEST_NAME##_Private_Test) { TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)<PrivateTestNetworkTraits>(); } \
	template<typename TTraits> void TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)()

	TRAITS_BASED_TEST(NemesisKeyProducesExpectedAddress) {
		// Act:
		auto address = PrivateKeyStringToAddressString(TTraits::Nemesis_Private_Key, TTraits::Network_Identifier);

		// Assert:
		EXPECT_EQ(TTraits::Expected_Nemesis_Address, address);
	}

	TRAITS_BASED_TEST(PrivateKeysProduceExpectedAddresses) {
		// Arrange:
		auto expectedAddresses = TTraits::ExpectedAddresses();

		// Act + Assert:
		auto i = 0u;
		for (const auto& pkString : TTraits::PrivateKeys()) {
			const auto& expectedAddress = expectedAddresses[i++];
			EXPECT_EQ(expectedAddress, PrivateKeyStringToAddressString(pkString, TTraits::Network_Identifier));
		}
	}
}
