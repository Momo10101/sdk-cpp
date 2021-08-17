import re

# those files won't be checked at all
SKIP_FILES = (
    # macro-based enums
    re.compile(r'src.symbol.core.utils.MacroBasedEnum.h'),

    # inline includes
    re.compile(r'src.symbol.core.model.EntityType.cpp'),
    re.compile(r'src.symbol.core.model.ReceiptType.cpp'),
)

NAMESPACES_FALSEPOSITIVES = (
    # multiple namespaces (specialization)
    re.compile(r'src.symbol.core.utils.Logging.cpp'),  # (boost::log)
    re.compile(r'tests.TestHarness.h'),  # (std)

    # disallowed top-level namespaces
    re.compile(r'src.symbol.core.thread.detail.FutureSharedState.h'),  # (detail)

    # no types (only includes and/or fwd declares and/or defines)
    re.compile(r'src.symbol.constants.h'),
    re.compile(r'src.symbol.plugins.h'),
    re.compile(r'src.symbol.preprocessor.h'),
    re.compile(r'src.symbol.core.utils.BitwiseEnum.h'),
    re.compile(r'src.symbol.core.utils.ExceptionLogging.h'),
    re.compile(r'src.symbol.core.utils.MacroBasedEnumIncludes.h'),
    re.compile(r'src.symbol.version.version_inc.h'),


    re.compile(r'src.symbol.txes.mosaic.MosaicConstants.h'),
    re.compile(r'src.symbol.txes.namespace.NamespaceConstants.h'),
    re.compile(r'tests.shared.nodeps.Stress.h'),
    re.compile(r'internal.tools.*Generators.h'),

    # main entry points
    re.compile(r'tests.bench.nodeps.BenchMain.cpp'),

    # mongo plugins (only entry point)
    re.compile(r'extensions.mongo.plugins.*.src.Mongo.*Plugin.cpp')
)

EMPTYLINES_FALSEPOSITIVES = (
)

LONGLINES_FALSEPOSITIVES = (
)

SPECIAL_INCLUDES = (
    # src (these include double quotes because they have to match what is after `#include `)
    re.compile(r'"catapult/utils/MacroBasedEnum\.h"'),
    re.compile(r'"symbol/core/utils/MacroBasedEnum\.h"'),
    re.compile(r'"ReentrancyCheckReaderNotificationPolicy.h"'),

    re.compile(r'<ref10/crypto_verify_32.h>'),

    # those always should be in an ifdef
    re.compile(r'<dlfcn.h>'),
    re.compile(r'<io.h>'),
    re.compile(r'<mach/mach.h>'),
    re.compile(r'<psapi.h>'),
    re.compile(r'<stdexcept>'),
    re.compile(r'<sys/file.h>'),
    re.compile(r'<sys/resource.h>'),
    re.compile(r'<sys/time.h>'),
    re.compile(r'<unistd.h>'),
    re.compile(r'<windows.h>')
)

CORE_FIRSTINCLUDES = {
    # src
    'src/symbol/core/ionet/IoEnums.cpp': 'ConnectResult.h',
    'src/symbol/version/nix/what_version.cpp': 'symbol/version/version.h',

    # sdk
    'tests/builders/AliasBuilderTests.cpp':              'builders/AddressAliasBuilder.h',
    'tests/builders/AccountRestrictionBuilderTests.cpp': 'builders/AccountAddressRestrictionBuilder.h',
    'tests/builders/KeyLinkBuilderTests.cpp':            'builders/AccountKeyLinkBuilder.h',
    'tests/builders/MetadataBuilderTests.cpp':           'builders/AccountMetadataBuilder.h',


    # tests
    'tests/shared/nodeps/TestMain.cpp': 'symbol/core/utils/ConfigurationValueParsers.h',
}

PLUGINS_FIRSTINCLUDES = {

}

TOOLS_FIRSTINCLUDES = {

}

EXTENSION_FIRSTINCLUDES = {

}

SKIP_FORWARDS = (
    re.compile(r'src.symbol.core.utils.ClampedBaseValue.h'),
    re.compile(r'.*\.cpp$')
)

FILTER_NAMESPACES = (
    re.compile(r'.*detail'),
    re.compile(r'.*_types::'),
    re.compile(r'.*_types$'),
    re.compile(r'.*bson_stream$')
)
