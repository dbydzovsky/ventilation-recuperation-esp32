import createReducer from "./createReducer";
import {CurrentState, StateAction} from "../model/state";
import {ConfigurationActions, ConfigurationState} from "../model/configuration";
import {SettingsAction, SettingsActions, SettingsState} from "../model/settings";

let settingsState: SettingsState = {
    settings: {
        checkRecuperationRpm: true,
        checkVentilatorRpm: true,
        unblockingFansPeriod: 0,
        ventilatorMaxRpm: 0,
        recuperationMaxRpm: 0,
        syncForecastTolerateLastSuccessFor: 0,
        syncForecastInterval: 0,
        relayCooldown: 0,
        recuperationWaitForDirectionChange: 0,
        recuperationCycleDuration: 0,
    }
}

export const settings = createReducer<SettingsState>(settingsState, {
    [SettingsActions.GET_SETTINGS](state: SettingsState, action: SettingsAction): SettingsState {
        return {settings: action.payload}
    },
});
