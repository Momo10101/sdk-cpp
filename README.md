# SDK-CPP

The symbol core SDK which implements the low level functionality of a client.

## Package Organization

The sdk code is organized as follows:

| Folder name | Description |
| -------------|--------------|
| /external | External dependencies that are built with the sdk. |
| /src/symbolSDK/core | The core SDK functionality with no outer dependencies |
| /src/symbolSDK/extended | The extended SDK functionality with dependencies on core |
| /src/symbolSDK/plugins | Modules that introduce new and different ways to alter the chain's state via transactions. |
| /tests | Collection of unit tests that test the sdk functionality. |
| /delete | Temporary, will be deleted soon. Contains code from catapult-client.|


## Compiling the SDK

- git clone https://github.com/symbol/sdk-cpp.git
- cd sdk-cpp
- mkdir build
- cd build
- cmake ..
- cmake  --build .

## Running unit tests

From the 'build' dir:

- bin/tests.catapult.parsers
- bin/tests.catapult.extensions
- bin/tests.catapult.builders

## License

Copyright (c) 2018 Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp Licensed under the [GNU Lesser General Public License v3](LICENSE.txt)

[developer documentation]: https://docs.symbolplatform.com/
[forum]: https://forum.nem.io/c/announcement
[issues]: https://github.com/symbol/sdk-java/issues
[discord]: https://discord.gg/xymcity
[catapult-client]: https://github.com/symbol/sdk-cpp
[symbol-rest]: https://github.com/symbol/catapult-rest
[nem]: https://nem.io
[symbol]: https://symbolplatform.com/

