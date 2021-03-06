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

#include "symbol/core/crypto/Signer.h"
#include "symbol/core/utils/HexParser.h"
#include "symbol/core/utils/RandomGenerator.h"
#include "tests/shared/crypto/CurveUtils.h"
#include "tests/shared/crypto/SignVerifyTests.h"
#include "tests/shared/nodeps/KeyTestUtils.h"
#include "tests/TestHarness.h"
#include <numeric>

namespace catapult { namespace crypto {

#define TEST_CLASS SignerTests

	// region basic sign verify tests

	namespace {
		struct SignVerifyTraits {
		public:
			using KeyPair = crypto::KeyPair;
			using Signature = catapult::Signature;

		public:
			static constexpr auto GenerateKeyPair = test::GenerateKeyPair;

			static auto GetPayloadForNonCanonicalSignatureTest() {
				// the value 30 in the payload ensures that the encodedS part of the signature is < 2 ^ 253 after adding the group order
				return std::array<uint8_t, 10>{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 30 } };
			}

			static auto MakeNonCanonical(const Signature& canonicalSignature) {
				// this is signature with group order added to 'encodedS' part of signature
				auto nonCanonicalSignature = canonicalSignature;
				test::ScalarAddGroupOrder(nonCanonicalSignature.data() + Signature::Size / 2);
				return nonCanonicalSignature;
			}
		};
	}

	DEFINE_SIGN_VERIFY_TESTS(SignVerifyTraits)

	// endregion

	// region VerifyMulti

	namespace {
		constexpr auto Default_Signature_Count = 100u;
		const Key Valid_Public_Key = utils::ParseByteArray<Key>("53C659B47C176A70EB228DE5C0A0FF391282C96640C2A42CD5BBD0982176AB1B");

		struct DataHolder {
			std::vector<Key> PublicKeys;
			std::vector<std::vector<uint8_t>> Buffers;
			std::vector<Signature> Signatures;
		};

		std::vector<SignatureInput> CreateSignatureInputs(size_t count, DataHolder& dataHolder) {
			std::vector<KeyPair> keyPairs;
			std::vector<SignatureInput> signatureInputs;
			dataHolder.PublicKeys.reserve(count);
			dataHolder.Signatures.reserve(count);

			for (auto i = 0u; i < count; ++i) {
				keyPairs.push_back(KeyPair::FromPrivate(PrivateKey::Generate(test::RandomByte)));
				auto& buffers = dataHolder.Buffers;
				auto& signatures = dataHolder.Signatures;
				dataHolder.PublicKeys.push_back(keyPairs.back().publicKey());
				buffers.push_back(test::GenerateRandomVector(50));
				buffers.push_back(test::GenerateRandomVector(70));
				signatures.push_back(Signature());
				Sign(keyPairs[i], { buffers[2 * i], buffers[2 * i + 1] }, signatures[i]);
				signatureInputs.push_back({ dataHolder.PublicKeys[i], { buffers[2 * i], buffers[2 * i + 1] }, signatures[i] });
			}

			return signatureInputs;
		}

		template<typename TArray>
		Signature SignPayload(const KeyPair& keyPair, const TArray& payload) {
			Signature signature;
			EXPECT_NO_THROW(Sign(keyPair, payload, signature));
			return signature;
		}

		template<typename TTraits>
		void AssertSignedPayloadsCanBeVerifiedAsBatches(size_t count) {
			// Arrange:
			DataHolder dataHolder;
			auto signatureInputs = CreateSignatureInputs(count, dataHolder);

			// Act:
			auto result = TTraits::Verify(signatureInputs);

			// Assert:
			auto failedIndexes = std::unordered_set<size_t>();
			TTraits::AssertVerifyResult(result, true, failedIndexes);
		}

		template<typename TTraits, typename TMutator>
		void AssertSignedPayloadsCannotBeVerifiedAsBatches(TMutator mutator) {
			// Arrange:
			DataHolder dataHolder;
			auto signatureInputs = CreateSignatureInputs(Default_Signature_Count, dataHolder);
			std::unordered_set<size_t> failedIndexes{ 1, 17, 58 };
			for (auto index : failedIndexes)
				mutator(signatureInputs, index);

			// Act:
			auto result = TTraits::Verify(signatureInputs);

			// Assert:
			TTraits::AssertVerifyResult(result, false, failedIndexes);
		}

		RandomFiller CreateRandomFiller() {
			return [](auto* pOut, auto count) {
				// can use low entropy source for tests
				utils::LowEntropyRandomGenerator().fill(pOut, count);
			};
		}

		struct VerifyMultiTraits {
			static std::pair<std::vector<bool>, bool> Verify(const std::vector<SignatureInput>& signatureInputs) {
				return VerifyMulti(CreateRandomFiller(), signatureInputs.data(), signatureInputs.size());
			}

			static void AssertVerifyResult(
					const std::pair<std::vector<bool>, bool>& result,
					bool expectedAggregateResult,
					std::unordered_set<size_t>& failedIndexes) {
				// Assert:
				EXPECT_EQ(expectedAggregateResult, result.second);

				for (auto i = 0u; i < result.first.size(); ++i) {
					if (failedIndexes.cend() != failedIndexes.find(i)) {
						EXPECT_FALSE(result.first[i]) << "at index " << i;
						failedIndexes.erase(i);
					} else {
						EXPECT_TRUE(result.first[i]) << "at index " << i;
					}
				}

				EXPECT_TRUE(failedIndexes.empty());
			}
		};

		struct VerifyMultiShortCircuitTraits {
			static bool Verify(const std::vector<SignatureInput>& signatureInputs) {
				return VerifyMultiShortCircuit(CreateRandomFiller(), signatureInputs.data(), signatureInputs.size());
			}

			static void AssertVerifyResult(bool result, bool expectedAggregateResult, std::unordered_set<size_t>&) {
				EXPECT_EQ(expectedAggregateResult, result);
			}
		};
	}

#define VERIFY_MULTI_TEST(TEST_NAME) \
	template<typename TTraits> void TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)(); \
	TEST(TEST_CLASS, TEST_NAME##_All) { TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)<VerifyMultiTraits>(); } \
	TEST(TEST_CLASS, TEST_NAME##_ShortCircuit) { TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)<VerifyMultiShortCircuitTraits>(); } \
	template<typename TTraits> void TRAITS_TEST_NAME(TEST_CLASS, TEST_NAME)()

	VERIFY_MULTI_TEST(SignedPayloadsCanBeVerifiedAsBatches_LessThanBatchSize) {
		AssertSignedPayloadsCanBeVerifiedAsBatches<TTraits>(0);
		AssertSignedPayloadsCanBeVerifiedAsBatches<TTraits>(1);
		AssertSignedPayloadsCanBeVerifiedAsBatches<TTraits>(63);
	}

	VERIFY_MULTI_TEST(SignedPayloadsCanBeVerifiedAsBatches_EqualToBatchSize) {
		AssertSignedPayloadsCanBeVerifiedAsBatches<TTraits>(64);
	}

	VERIFY_MULTI_TEST(SignedPayloadsCanBeVerifiedAsBatches_GreaterThanBatchSize) {
		AssertSignedPayloadsCanBeVerifiedAsBatches<TTraits>(65); // last signature is not batch verified
		AssertSignedPayloadsCanBeVerifiedAsBatches<TTraits>(100); // 2 batches
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_DifferentKey) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			const_cast<Key&>(signatureInputs[index].PublicKey) = Valid_Public_Key;
		});
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_DifferentRPart) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			const_cast<Signature&>(signatureInputs[index].Signature)[5] ^= 0xFF;
		});
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_DifferentSPart) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			const_cast<Signature&>(signatureInputs[index].Signature)[47] ^= 0xFF;
		});
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_DifferentPayload) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			const_cast<uint8_t*>(signatureInputs[index].Buffers[0].pData)[13] ^= 0xFF;
		});
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_PublicKeyNotOnCurve) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			auto& publicKey = const_cast<Key&>(signatureInputs[index].PublicKey);
			std::fill(publicKey.begin(), publicKey.end(), static_cast<uint8_t>(0));
			publicKey[publicKey.size() - 1] = 0x01;
		});
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_ZeroPublicKey) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			const_cast<Key&>(signatureInputs[index].PublicKey) = Key();
		});
	}

	VERIFY_MULTI_TEST(SignedPayloadsCannotBeVerifiedAsBatches_NonCanonicalSignature) {
		AssertSignedPayloadsCannotBeVerifiedAsBatches<TTraits>([](auto& signatureInputs, auto index) {
			auto payload = SignVerifyTraits::GetPayloadForNonCanonicalSignatureTest();
			auto nonCanonicalSignature = SignVerifyTraits::MakeNonCanonical(SignPayload(test::GenerateKeyPair(), payload));

			const_cast<Signature&>(signatureInputs[index].Signature) = nonCanonicalSignature;
		});
	}

	// endregion

	// region test vectors

	namespace {
		struct TestVectorsInput {
			std::vector<std::string> InputData;
			std::vector<std::string> PrivateKeys;
			std::vector<std::string> ExpectedPublicKeys;
			std::vector<std::string> ExpectedSignatures;
		};

		// test vectors from rfc8032
		TestVectorsInput GetTestVectorsInput() {
			TestVectorsInput input;
			input.InputData = {
				"",
				"72",
				"AF82",

				// long 1023-byte msg
				"08B8B2B733424243760FE426A4B54908632110A66C2F6591EABD3345E3E4EB98"
				"FA6E264BF09EFE12EE50F8F54E9F77B1E355F6C50544E23FB1433DDF73BE84D8"
				"79DE7C0046DC4996D9E773F4BC9EFE5738829ADB26C81B37C93A1B270B20329D"
				"658675FC6EA534E0810A4432826BF58C941EFB65D57A338BBD2E26640F89FFBC"
				"1A858EFCB8550EE3A5E1998BD177E93A7363C344FE6B199EE5D02E82D522C4FE"
				"BA15452F80288A821A579116EC6DAD2B3B310DA903401AA62100AB5D1A36553E"
				"06203B33890CC9B832F79EF80560CCB9A39CE767967ED628C6AD573CB116DBEF"
				"EFD75499DA96BD68A8A97B928A8BBC103B6621FCDE2BECA1231D206BE6CD9EC7"
				"AFF6F6C94FCD7204ED3455C68C83F4A41DA4AF2B74EF5C53F1D8AC70BDCB7ED1"
				"85CE81BD84359D44254D95629E9855A94A7C1958D1F8ADA5D0532ED8A5AA3FB2"
				"D17BA70EB6248E594E1A2297ACBBB39D502F1A8C6EB6F1CE22B3DE1A1F40CC24"
				"554119A831A9AAD6079CAD88425DE6BDE1A9187EBB6092CF67BF2B13FD65F270"
				"88D78B7E883C8759D2C4F5C65ADB7553878AD575F9FAD878E80A0C9BA63BCBCC"
				"2732E69485BBC9C90BFBD62481D9089BECCF80CFE2DF16A2CF65BD92DD597B07"
				"07E0917AF48BBB75FED413D238F5555A7A569D80C3414A8D0859DC65A46128BA"
				"B27AF87A71314F318C782B23EBFE808B82B0CE26401D2E22F04D83D1255DC51A"
				"DDD3B75A2B1AE0784504DF543AF8969BE3EA7082FF7FC9888C144DA2AF58429E"
				"C96031DBCAD3DAD9AF0DCBAAAF268CB8FCFFEAD94F3C7CA495E056A9B47ACDB7"
				"51FB73E666C6C655ADE8297297D07AD1BA5E43F1BCA32301651339E22904CC8C"
				"42F58C30C04AAFDB038DDA0847DD988DCDA6F3BFD15C4B4C4525004AA06EEFF8"
				"CA61783AACEC57FB3D1F92B0FE2FD1A85F6724517B65E614AD6808D6F6EE34DF"
				"F7310FDC82AEBFD904B01E1DC54B2927094B2DB68D6F903B68401ADEBF5A7E08"
				"D78FF4EF5D63653A65040CF9BFD4ACA7984A74D37145986780FC0B16AC451649"
				"DE6188A7DBDF191F64B5FC5E2AB47B57F7F7276CD419C17A3CA8E1B939AE49E4"
				"88ACBA6B965610B5480109C8B17B80E1B7B750DFC7598D5D5011FD2DCC5600A3"
				"2EF5B52A1ECC820E308AA342721AAC0943BF6686B64B2579376504CCC493D97E"
				"6AED3FB0F9CD71A43DD497F01F17C0E2CB3797AA2A2F256656168E6C496AFC5F"
				"B93246F6B1116398A346F1A641F3B041E989F7914F90CC2C7FFF357876E506B5"
				"0D334BA77C225BC307BA537152F3F1610E4EAFE595F6D9D90D11FAA933A15EF1"
				"369546868A7F3A45A96768D40FD9D03412C091C6315CF4FDE7CB68606937380D"
				"B2EAAA707B4C4185C32EDDCDD306705E4DC1FFC872EEEE475A64DFAC86ABA41C"
				"0618983F8741C5EF68D3A101E8A3B8CAC60C905C15FC910840B94C00A0B9D0",
				// test vector sha(abc)
				"DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A"
				"2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F"
			};

			input.PrivateKeys = {
				"9D61B19DEFFD5A60BA844AF492EC2CC44449C5697B326919703BAC031CAE7F60",
				"4CCD089B28FF96DA9DB6C346EC114E0F5B8A319F35ABA624DA8CF6ED4FB8A6FB",
				"C5AA8DF43F9F837BEDB7442F31DCB7B166D38535076F094B85CE3A2E0B4458F7",
				"F5E5767CF153319517630F226876B86C8160CC583BC013744C6BF255F5CC0EE5",
				"833FE62409237B9D62EC77587520911E9A759CEC1D19755B7DA901B96DCA3D42"
			};
			input.ExpectedPublicKeys = {
				"D75A980182B10AB7D54BFED3C964073A0EE172F3DAA62325AF021A68F707511A",
				"3D4017C3E843895A92B70AA74D1B7EBC9C982CCF2EC4968CC0CD55F12AF4660C",
				"FC51CD8E6218A1A38DA47ED00230F0580816ED13BA3303AC5DEB911548908025",
				"278117FC144C72340F67D0F2316E8386CEFFBF2B2428C9C51FEF7C597F1D426E",
				"EC172B93AD5E563BF4932C70E1245034C35467EF2EFD4D64EBF819683467E2BF"
			};
			input.ExpectedSignatures = {
				"E5564300C360AC729086E2CC806E828A84877F1EB8E5D974D873E06522490155"
				"5FB8821590A33BACC61E39701CF9B46BD25BF5F0595BBE24655141438E7A100B",
				"92A009A9F0D4CAB8720E820B5F642540A2B27B5416503F8FB3762223EBDB69DA"
				"085AC1E43E15996E458F3613D0F11D8C387B2EAEB4302AEEB00D291612BB0C00",
				"6291D657DEEC24024827E69C3ABE01A30CE548A284743A445E3680D7DB5AC3AC"
				"18FF9B538D16F290AE67F760984DC6594A7C15E9716ED28DC027BECEEA1EC40A",
				"0AAB4C900501B3E24D7CDF4663326A3A87DF5E4843B2CBDB67CBF6E460FEC350"
				"AA5371B1508F9F4528ECEA23C436D94B5E8FCD4F681E30A6AC00A9704A188A03",
				"DC2A4459E7369633A52B1BF277839A00201009A3EFBF3ECB69BEA2186C26B589"
				"09351FC9AC90B3ECFDFBC7C66431E0303DCA179C138AC17AD9BEF1177331A704"
			};

			// Sanity:
			EXPECT_EQ(input.InputData.size(), input.PrivateKeys.size());
			EXPECT_EQ(input.InputData.size(), input.ExpectedPublicKeys.size());
			EXPECT_EQ(input.InputData.size(), input.ExpectedSignatures.size());
			return input;
		}
	}

	TEST(TEST_CLASS, SignPassesTestVectors) {
		// Arrange:
		auto input = GetTestVectorsInput();

		// Act / Assert:
		for (auto i = 0u; i < input.InputData.size(); ++i) {
			// Act:
			auto keyPair = KeyPair::FromString(input.PrivateKeys[i]);
			auto signature = SignPayload(keyPair, test::HexStringToVector(input.InputData[i]));

			// Assert:
			auto message = "test vector at " + std::to_string(i);
			EXPECT_EQ(utils::ParseByteArray<Key>(input.ExpectedPublicKeys[i]), keyPair.publicKey()) << message;
			EXPECT_EQ(utils::ParseByteArray<Signature>(input.ExpectedSignatures[i]), signature) << message;
		}
	}

	TEST(TEST_CLASS, VerifyPassesTestVectors_Verify) {
		// Arrange:
		auto input = GetTestVectorsInput();

		// Act / Assert:
		for (auto i = 0u; i < input.InputData.size(); ++i) {
			// Act:
			auto keyPair = KeyPair::FromString(input.PrivateKeys[i]);
			auto payload = test::HexStringToVector(input.InputData[i]);
			auto signature = SignPayload(keyPair, payload);
			auto isVerified = Verify(keyPair.publicKey(), payload, signature);

			// Assert:
			auto message = "test vector at " + std::to_string(i);
			EXPECT_TRUE(isVerified) << message;
		}
	}

	TEST(TEST_CLASS, VerifyPassesTestVectors_VerifyMulti) {
		// Arrange:
		auto input = GetTestVectorsInput();
		DataHolder dataHolder;
		dataHolder.PublicKeys.reserve(input.InputData.size());
		dataHolder.Signatures.reserve(input.InputData.size());
		std::vector<SignatureInput> signatureInputs;

		for (auto i = 0u; i < input.InputData.size(); ++i) {
			auto keyPair = KeyPair::FromString(input.PrivateKeys[i]);
			dataHolder.PublicKeys.push_back(keyPair.publicKey());
			dataHolder.Buffers.push_back(test::HexStringToVector(input.InputData[i]));
			dataHolder.Signatures.push_back(SignPayload(keyPair, dataHolder.Buffers.back()));
			signatureInputs.push_back({ dataHolder.PublicKeys.back(), { dataHolder.Buffers.back() }, dataHolder.Signatures.back() });
		}

		// Act:
		auto pair = VerifyMulti(CreateRandomFiller(), signatureInputs.data(), signatureInputs.size());

		// Assert:
		EXPECT_TRUE(pair.second);

		for (auto i = 0u; i < input.InputData.size(); ++i) {
			auto message = "test vector at " + std::to_string(i);
			EXPECT_TRUE(pair.first[i]) << message;
		}
	}

	// endregion
}}
