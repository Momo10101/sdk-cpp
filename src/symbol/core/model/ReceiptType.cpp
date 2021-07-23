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

#include "ReceiptType.h"
#include "symbol/core/utils/MacroBasedEnumIncludes.h"

namespace catapult { namespace model {

#define DEFINE_CASE(RESULT) case utils::to_underlying_type(RESULT)

#define CASE_WELL_KNOWN_RECEIPT_TYPE(NAME) DEFINE_CASE(Receipt_Type_##NAME): return #NAME

#define CUSTOM_RECEIPT_TYPE_DEFINITION 1
#undef DEFINE_RECEIPT_TYPE

#define STR(SYMBOL) #SYMBOL

#define DEFINE_RECEIPT_TYPE(BASIC_TYPE, FACILITY, DESCRIPTION, CODE) \
	DEFINE_CASE(MakeReceiptType((model::BasicReceiptType::BASIC_TYPE), (model::FacilityCode::FACILITY), CODE)): \
		return STR(DESCRIPTION)

	namespace {
		const char* ToString(ReceiptType receiptType) {
			switch (utils::to_underlying_type(receiptType)) {
			// well known types defined in ReceiptType.h
			CASE_WELL_KNOWN_RECEIPT_TYPE(Harvest_Fee);
			CASE_WELL_KNOWN_RECEIPT_TYPE(Inflation);
			CASE_WELL_KNOWN_RECEIPT_TYPE(Transaction_Group);
			CASE_WELL_KNOWN_RECEIPT_TYPE(Address_Alias_Resolution);
			CASE_WELL_KNOWN_RECEIPT_TYPE(Mosaic_Alias_Resolution);

			// plugin receipt types
			#include "symbol/txes/lock_hash/HashLockReceiptType.h"
			#include "symbol/txes/lock_secret/SecretLockReceiptType.h"
			#include "symbol/txes/mosaic/MosaicReceiptType.h"
			#include "symbol/txes/namespace/NamespaceReceiptType.h"
			}

			return nullptr;
		}
	}

	std::ostream& operator<<(std::ostream& out, ReceiptType receiptType) {
		auto pStr = ToString(receiptType);
		if (pStr)
			out << pStr;
		else
			out << "ReceiptType<0x" << utils::HexFormat(utils::to_underlying_type(receiptType)) << ">";

		return out;
	}
}}
