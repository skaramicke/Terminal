/********************************************************
 *                                                       *
 *   Copyright (C) Microsoft. All rights reserved.       *
 *                                                       *
 ********************************************************/

#include "pch.h"
#include "GlobalAppSettings.h"
#include "../../types/inc/Utils.hpp"
#include <DefaultSettings.h>

using namespace Microsoft::Terminal::TerminalApp;
using namespace winrt::Microsoft::Terminal::Settings;
using namespace winrt::Microsoft::Terminal::TerminalApp;
using namespace winrt::Windows::Data::Json;
using namespace ::Microsoft::Console;

static const std::wstring DEFAULTPROFILE_KEY{ L"defaultProfile" };
static const std::wstring ALWAYS_SHOW_TABS_KEY{ L"alwaysShowTabs" };
static const std::wstring INITIALROWS_KEY{ L"initialRows" };
static const std::wstring INITIALCOLS_KEY{ L"initialCols" };


GlobalAppSettings::GlobalAppSettings() :
    _keybindings{},
    _colorSchemes{},
    _defaultProfile{},
    _alwaysShowTabs{ false },
    _initialRows{ DEFAULT_ROWS },
    _initialCols{ DEFAULT_COLS }
{

}

GlobalAppSettings::~GlobalAppSettings()
{

}

const std::vector<ColorScheme>& GlobalAppSettings::GetColorSchemes() const noexcept
{
    return _colorSchemes;
}


std::vector<ColorScheme>& GlobalAppSettings::GetColorSchemes() noexcept
{
    return _colorSchemes;
}

void GlobalAppSettings::SetDefaultProfile(const GUID defaultProfile) noexcept
{
    _defaultProfile = defaultProfile;
}

GUID GlobalAppSettings::GetDefaultProfile() const noexcept
{
    return _defaultProfile;
}

AppKeyBindings GlobalAppSettings::GetKeybindings() const noexcept
{
    return _keybindings;
}

bool GlobalAppSettings::GetAlwaysShowTabs() const noexcept
{
    return _alwaysShowTabs;
}

void GlobalAppSettings::SetAlwaysShowTabs(const bool showTabs) noexcept
{
    _alwaysShowTabs = showTabs;
}

// Method Description:
// - Applies appropriate settings from the globals into the given TerminalSettings.
// Arguments:
// - settings: a TerminalSettings object to add global property values to.
// Return Value:
// - <none>
void GlobalAppSettings::ApplySettings(TerminalSettings& settings) const noexcept
{
    settings.KeyBindings(GetKeybindings());
    settings.InitialRows(_initialRows);
    settings.InitialCols(_initialCols);
}

// Method Description:
// - Serialize this object to a JsonObject.
// Arguments:
// - <none>
// Return Value:
// - a JsonObject which is an equivalent serialization of this object.
JsonObject GlobalAppSettings::ToJson() const
{
    winrt::Windows::Data::Json::JsonObject jsonObject;

    const auto guidStr = Utils::GuidToString(_defaultProfile);
    const auto defaultProfile = JsonValue::CreateStringValue(guidStr);
    const auto initialRows = JsonValue::CreateNumberValue(_initialRows);
    const auto initialCols = JsonValue::CreateNumberValue(_initialCols);

    jsonObject.Insert(DEFAULTPROFILE_KEY, defaultProfile);
    jsonObject.Insert(INITIALROWS_KEY, initialRows);
    jsonObject.Insert(INITIALCOLS_KEY, initialCols);
    jsonObject.Insert(ALWAYS_SHOW_TABS_KEY,
                      JsonValue::CreateBooleanValue(_alwaysShowTabs));

    return jsonObject;
}

// Method Description:
// - Create a new instance of this class from a serialized JsonObject.
// Arguments:
// - json: an object which should be a serialization of a GlobalAppSettings object.
// Return Value:
// - a new GlobalAppSettings instance created from the values in `json`
GlobalAppSettings GlobalAppSettings::FromJson(winrt::Windows::Data::Json::JsonObject json)
{
    GlobalAppSettings result{};

    if (json.HasKey(DEFAULTPROFILE_KEY))
    {
        auto guidString = json.GetNamedString(DEFAULTPROFILE_KEY);
        auto guid = Utils::GuidFromString(guidString.c_str());
        result._defaultProfile = guid;
    }

    if (json.HasKey(ALWAYS_SHOW_TABS_KEY))
    {
        result._alwaysShowTabs = json.GetNamedBoolean(ALWAYS_SHOW_TABS_KEY);
    }
    if (json.HasKey(INITIALROWS_KEY))
    {
        result._initialRows = static_cast<int32_t>(json.GetNamedNumber(INITIALROWS_KEY));
    }
    if (json.HasKey(INITIALCOLS_KEY))
    {
        result._initialCols = static_cast<int32_t>(json.GetNamedNumber(INITIALCOLS_KEY));
    }

    return result;
}
