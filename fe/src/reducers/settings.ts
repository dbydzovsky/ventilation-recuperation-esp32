import createReducer from "./createReducer";
import {ClearSettingsActions, SettingsAction, SettingsActions, SettingsState} from "../model/settings";

let settingsState: SettingsState = {
    settings: {
        testingMode: false,
        recuperationOn: true,
        ventilatorRevolutions: 3,
        recuperationRevolutions: 2,
        checkRecuperationRpm: true,
        checkVentilatorRpm: true,
        hideCo2: false,
        hideInternalTempHum: false,
        unblockingFansPeriod: 0,
        ventilatorMaxRpm: 0,
        recuperationMaxRpm: 0,
        recuperationWaitForDirectionChange: 0,
        recuperationCycleDuration: 0,
        recuperationMode: 1,
        recuperationPower: 100,
        ventilatorPower: 0,
        durationMillis: 300000,
        tempDisableDuration: 10800000,
        brightness: 100,
        recuperationMhz: 25000,
        ventilationMhz: 25000,
        ventilationRelayPin: 0,
        recuperationRelayPin: 0
    },
    counter: 1,
    connected: true,
}

export const settings = createReducer<SettingsState>(settingsState, {
    [SettingsActions.SET_SETTINGS](state: SettingsState, action: SettingsAction): SettingsState {
        return {settings: action.payload, counter: state.counter + 1, connected: true}
    },
    [SettingsActions.CLEAR_SETTINGS](state: SettingsState, action: ClearSettingsActions): SettingsState {
        // if (!state.connected && action.payload.connected) {
        //     return {...state, counter: state.counter+1, connected: true}
        // } else if (state.connected && !action.payload.connected){
        //     return {...state, connected: false}
        // }
        return state
    },
});
