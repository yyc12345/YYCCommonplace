#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/binstore.hpp>
#include <yycc/prelude.hpp>
#include <sstream>

#define BINSTORE ::yycc::carton::binstore

namespace yycctest::carton::binstore {

    using Token = BINSTORE::types::Token;
    using VersionIdentifier = BINSTORE::types::VersionIdentifier;
    using Setting = BINSTORE::setting::Setting;
    using SettingCollection = BINSTORE::setting::SettingCollection;
    using Configuration = BINSTORE::configuration::Configuration;
    using Storage = BINSTORE::storage::Storage;
    using LoadStrategy = BINSTORE::storage::LoadStrategy;
    namespace serdes = BINSTORE::serdes;

#pragma region Binstore Builder

    // YYC MARK:
    // Due to the same reason like clap,
    // I rename them with new test suit name.

    TEST(CartonBinstoreSetting, BadSetting) {
        // Test that we can create settings with names
        EXPECT_NO_THROW(Setting setting(u8"test-setting"));
        // Test that we cannot create settings with empty names
        EXPECT_ANY_THROW(Setting setting(u8""));
    }

    TEST(CartonBinstoreSetting, BadSettings) {
        // Test that duplicate names are not allowed
        auto settings = SettingCollection();
        auto token1 = settings.add_setting(Setting(u8"setting1"));
        EXPECT_ANY_THROW(settings.add_setting(Setting(u8"setting1")));
    }

#pragma endregion

#pragma region Binstore Operations

    enum class TestEnum : int8_t { Test1, Test2, Test3 };

    class CartonBinstore : public ::testing::Test {
    protected:
        CartonBinstore() :
            // YYC MARK:
            // Due to the same reason like clap, I need initialize a blank configuration.
            configuration(0, SettingCollection()) {
            // Create settings
            auto settings = SettingCollection();
            int_setting = settings.add_setting(Setting(u8"int-setting"));
            float_setting = settings.add_setting(Setting(u8"float-setting"));
            string_setting = settings.add_setting(Setting(u8"string-setting"));
            bool_setting = settings.add_setting(Setting(u8"bool-setting"));
            clamped_float_setting = settings.add_setting(Setting(u8"clamped-float-setting"));
            enum_setting = settings.add_setting(Setting(u8"enum-setting"));

            // Create configuration
            configuration = Configuration(0, std::move(settings));
        }
        ~CartonBinstore() override = default;

        /**
         * @brief Build Storage with given Configuration.
         * @remarks The Configuration passed to Storage is a copy of this class member.
         * @return Built Storage.
         */
        Storage build_storage() const { return Storage(Configuration(this->configuration)); }

        // Tokens for accessing settings
        Token int_setting;
        using IntSettingSerDes = serdes::IntegralSerDes<i32>;
        Token float_setting;
        using FloatSettingSerDes = serdes::FloatingPointSerDes<float>;
        Token string_setting;
        using StringSettingSerDes = serdes::StringSerDes;
        Token bool_setting;
        using BoolSettingSerDes = serdes::BoolSerDes<false>;
        Token clamped_float_setting;
        using ClampedFloatSettingSerDes = serdes::FloatingPointSerDes<float, 0.0f, -1.0f, 1.0f>;
        Token enum_setting;
        using EnumSettingSerDes = serdes::EnumSerDes<TestEnum, TestEnum::Test1>;

        // Configuration
        Configuration configuration;
    };

    TEST_F(CartonBinstore, Configuration) {
        // Test version
        EXPECT_EQ(configuration.get_version(), 0);

        // Test settings access
        const auto& settings = configuration.get_settings();
        EXPECT_EQ(settings.length(), 6);
        EXPECT_TRUE(settings.has_setting(int_setting));
        EXPECT_TRUE(settings.has_setting(float_setting));
        EXPECT_TRUE(settings.has_setting(string_setting));
        EXPECT_TRUE(settings.has_setting(bool_setting));
        EXPECT_TRUE(settings.has_setting(clamped_float_setting));
        EXPECT_TRUE(settings.has_setting(enum_setting));
        EXPECT_NO_THROW(settings.get_setting(int_setting));
        EXPECT_NO_THROW(settings.get_setting(float_setting));
        EXPECT_NO_THROW(settings.get_setting(string_setting));
        EXPECT_NO_THROW(settings.get_setting(bool_setting));
        EXPECT_NO_THROW(settings.get_setting(clamped_float_setting));
        EXPECT_NO_THROW(settings.get_setting(enum_setting));
    }

    TEST_F(CartonBinstore, StorageBasicOperations) {
        auto storage = build_storage();

        // Test that we can reset values to defaults
        EXPECT_NO_THROW(storage.reset_value<IntSettingSerDes>(int_setting));
        EXPECT_NO_THROW(storage.reset_value<FloatSettingSerDes>(float_setting));
        EXPECT_NO_THROW(storage.reset_value<StringSettingSerDes>(string_setting));
        EXPECT_NO_THROW(storage.reset_value<BoolSettingSerDes>(bool_setting));
        EXPECT_NO_THROW(storage.reset_value<ClampedFloatSettingSerDes>(clamped_float_setting));
        EXPECT_NO_THROW(storage.reset_value<EnumSettingSerDes>(enum_setting));
    }

    TEST_F(CartonBinstore, StorageSetValueGetValue) {
        auto storage = build_storage();

        // Test setting and getting values
        // Integer setting
        {
            i32 test_value = 114;
            bool set_success = storage.set_value<IntSettingSerDes>(int_setting, test_value);
            EXPECT_TRUE(set_success);

            auto retrieved_value = storage.get_value<IntSettingSerDes>(int_setting);
            EXPECT_EQ(retrieved_value, test_value);
        }
        // Float setting
        {
            float test_value = 2.0f;
            bool set_success = storage.set_value<FloatSettingSerDes>(float_setting, test_value);
            EXPECT_TRUE(set_success);

            auto retrieved_value = storage.get_value<FloatSettingSerDes>(float_setting);
            EXPECT_EQ(retrieved_value, test_value);
        }
        // String setting
        {
            std::u8string test_value = u8"test";
            bool set_success = storage.set_value<StringSettingSerDes>(string_setting, test_value);
            EXPECT_TRUE(set_success);

            auto retrieved_value = storage.get_value<StringSettingSerDes>(string_setting);
            EXPECT_EQ(retrieved_value, test_value);
        }
        // Boolean setting
        {
            bool test_value = true;
            bool set_success = storage.set_value<BoolSettingSerDes>(bool_setting, test_value);
            EXPECT_TRUE(set_success);

            auto retrieved_value = storage.get_value<BoolSettingSerDes>(bool_setting);
            EXPECT_EQ(retrieved_value, test_value);
        }
        // Clamped float setting (within range)
        {
            float test_value = 0.5f;
            bool set_success = storage.set_value<ClampedFloatSettingSerDes>(clamped_float_setting, test_value);
            EXPECT_TRUE(set_success);

            auto retrieved_value = storage.get_value<ClampedFloatSettingSerDes>(clamped_float_setting);
            EXPECT_FLOAT_EQ(retrieved_value, test_value);
        }
        // Enum setting
        {
            TestEnum test_value = TestEnum::Test2;
            bool set_success = storage.set_value<EnumSettingSerDes>(enum_setting, test_value);
            EXPECT_TRUE(set_success);

            auto retrieved_value = storage.get_value<EnumSettingSerDes>(enum_setting);
            EXPECT_EQ(retrieved_value, test_value);
        }
    }

    TEST_F(CartonBinstore, StorageConstraintHandling) {
        auto storage = build_storage();

        // Test clamped float setting with out-of-range value
        // Should fail serialization and be reset to default
        float out_of_range_value = 2.0f; // Outside [-1.0f, 1.0f]
        bool set_success = storage.set_value<ClampedFloatSettingSerDes>(clamped_float_setting, out_of_range_value);
        EXPECT_FALSE(set_success); // Should return false indicating constraint violation

        // Value should be reset to default (0.0f)
        auto default_value = storage.get_value<ClampedFloatSettingSerDes>(clamped_float_setting);
        EXPECT_EQ(default_value, 0.0f);
    }

    TEST_F(CartonBinstore, StoragePersistence) {
        // Create storage
        auto storage = build_storage();

        // Prepare test value
        static i32 INT_VALUE = 114;
        static float FLOAT_VALUE = 2.0f;
        static std::u8string STRING_VALUE = u8"test";
        static bool BOOL_VALUE = true;
        static float CLAMPED_FLOAT_VALUE = 0.5f;
        static TestEnum ENUM_VALUE = TestEnum::Test2;

        // Set settings with given value.
        {
            ASSERT_TRUE(storage.set_value<IntSettingSerDes>(int_setting, INT_VALUE));
            ASSERT_TRUE(storage.set_value<FloatSettingSerDes>(float_setting, FLOAT_VALUE));
            ASSERT_TRUE(storage.set_value<StringSettingSerDes>(string_setting, STRING_VALUE));
            ASSERT_TRUE(storage.set_value<BoolSettingSerDes>(bool_setting, BOOL_VALUE));
            ASSERT_TRUE(storage.set_value<ClampedFloatSettingSerDes>(clamped_float_setting, CLAMPED_FLOAT_VALUE));
            ASSERT_TRUE(storage.set_value<EnumSettingSerDes>(enum_setting, ENUM_VALUE));
        }

        // Save it into buffer.
        // Use stringstream as buffer.
        std::stringstream ss;
        ASSERT_TRUE(storage.save(ss).has_value());

        // Clear IO bit flags, not the content.
        ss.clear();
        // Clear values
        storage.clear();

        // Load it into new created storage.
        ASSERT_TRUE(storage.load(ss, LoadStrategy::OnlyCurrent).has_value());

        // Check loaded settings
        {
            auto retrieved_value = storage.get_value<IntSettingSerDes>(int_setting);
            EXPECT_EQ(retrieved_value, INT_VALUE);
        }
        {
            auto retrieved_value = storage.get_value<FloatSettingSerDes>(float_setting);
            EXPECT_EQ(retrieved_value, FLOAT_VALUE);
        }
        {
            auto retrieved_value = storage.get_value<StringSettingSerDes>(string_setting);
            EXPECT_EQ(retrieved_value, STRING_VALUE);
        }
        {
            auto retrieved_value = storage.get_value<BoolSettingSerDes>(bool_setting);
            EXPECT_EQ(retrieved_value, BOOL_VALUE);
        }
        {
            auto retrieved_value = storage.get_value<ClampedFloatSettingSerDes>(clamped_float_setting);
            EXPECT_FLOAT_EQ(retrieved_value, CLAMPED_FLOAT_VALUE);
        }
        {
            auto retrieved_value = storage.get_value<EnumSettingSerDes>(enum_setting);
            EXPECT_EQ(retrieved_value, ENUM_VALUE);
        }
    }

#pragma endregion

#pragma region Binstore Version Handling

    class CartonBinstoreVersion : public ::testing::Test {
    protected:
        CartonBinstoreVersion() {
            // Create settings
            settings = SettingCollection();
            setting = settings.add_setting(Setting(u8"int-setting"));
        }
        ~CartonBinstoreVersion() override = default;

        /**
         * @brief Build Storage with given Settings.
         * @param[in] ver Version used by intermediate Configuration.
         * @remarks The Settings passed to Storage is a copy of this class member.
         * The intermediate Configuration is eaten by Storage.
         * @return Built Storage.
         */
        Storage build_storage(VersionIdentifier ver) const {
            auto cfg = Configuration(ver, SettingCollection(this->settings));
            return Storage(std::move(cfg));
        }

        /**
         * @brief Prepare a stringstream for loading.
         * @param[in] ss The string stream to be loaded in future.
         */
        void prepare_load(std::stringstream& ss) const {
            ss.clear();
            ss.seekg(0);
        }

        // Tokens for accessing settings
        Token setting;
        using SettingSerDes = serdes::IntegralSerDes<i32>;
        static constexpr i32 SETTING_VALUE = 42;

        // Settings
        SettingCollection settings;
    };

    TEST_F(CartonBinstoreVersion, LoadStrategy) {
        // Prepare buffer and essential settings
        constexpr VersionIdentifier OLD_VERSION = 16;
        constexpr VersionIdentifier MID_VERSION = 32;
        constexpr VersionIdentifier NEW_VERSION = 61;

        // Create old, mid, new storage buffer.
        std::stringstream ss_old, ss_mid, ss_new;
        auto storage_old = build_storage(OLD_VERSION);
        auto storage_mid = build_storage(MID_VERSION);
        auto storage_new = build_storage(NEW_VERSION);
        {
            storage_old.set_value<SettingSerDes>(setting, SETTING_VALUE);
            ASSERT_TRUE(storage_old.save(ss_old).has_value());
        }
        {
            storage_mid.set_value<SettingSerDes>(setting, SETTING_VALUE);
            ASSERT_TRUE(storage_mid.save(ss_mid).has_value());
        }
        {
            storage_new.set_value<SettingSerDes>(setting, SETTING_VALUE);
            ASSERT_TRUE(storage_new.save(ss_new).has_value());
        }

        // Try to load other configuration by OnlyCurrent strategy
        {
            storage_mid.clear();
            prepare_load(ss_old);
            ASSERT_FALSE(storage_mid.load(ss_old, LoadStrategy::OnlyCurrent).has_value());
        }
        {
            storage_mid.clear();
            prepare_load(ss_mid);
            ASSERT_TRUE(storage_mid.load(ss_mid, LoadStrategy::OnlyCurrent).has_value());
            EXPECT_EQ(storage_mid.get_value<SettingSerDes>(setting), SETTING_VALUE);
        }
        {
            storage_mid.clear();
            prepare_load(ss_new);
            ASSERT_FALSE(storage_mid.load(ss_new, LoadStrategy::OnlyCurrent).has_value());
        }
        // Try to load other configuration by MigrateOld strategy
        {
            storage_mid.clear();
            prepare_load(ss_old);
            ASSERT_TRUE(storage_mid.load(ss_old, LoadStrategy::MigrateOld).has_value());
            EXPECT_EQ(storage_mid.get_value<SettingSerDes>(setting), SETTING_VALUE);
        }
        {
            storage_mid.clear();
            prepare_load(ss_mid);
            ASSERT_TRUE(storage_mid.load(ss_mid, LoadStrategy::MigrateOld).has_value());
            EXPECT_EQ(storage_mid.get_value<SettingSerDes>(setting), SETTING_VALUE);
        }
        {
            storage_mid.clear();
            prepare_load(ss_new);
            ASSERT_FALSE(storage_mid.load(ss_new, LoadStrategy::MigrateOld).has_value());
        }
        // Try to load other configuration by AcceptAll strategy
        {
            storage_mid.clear();
            prepare_load(ss_old);
            ASSERT_TRUE(storage_mid.load(ss_old, LoadStrategy::AcceptAll).has_value());
            EXPECT_EQ(storage_mid.get_value<SettingSerDes>(setting), SETTING_VALUE);
        }
        {
            storage_mid.clear();
            prepare_load(ss_mid);
            ASSERT_TRUE(storage_mid.load(ss_mid, LoadStrategy::AcceptAll).has_value());
            EXPECT_EQ(storage_mid.get_value<SettingSerDes>(setting), SETTING_VALUE);
        }
        {
            storage_mid.clear();
            prepare_load(ss_new);
            ASSERT_TRUE(storage_mid.load(ss_new, LoadStrategy::AcceptAll).has_value());
            EXPECT_EQ(storage_mid.get_value<SettingSerDes>(setting), SETTING_VALUE);
        }
    }

#pragma endregion

} // namespace yycctest::carton::binstore