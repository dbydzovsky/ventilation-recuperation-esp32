import createReducer from "./createReducer";
import {CurrentState, StateAction} from "../model/state";
import {ConfigurationActions, ConfigurationState} from "../model/configuration";

let configurationState: ConfigurationState = {
    conf: undefined
}

export const configuration = createReducer<ConfigurationState>(configurationState, {
    [ConfigurationActions.GET_CONFIGURATION](state: CurrentState, action: StateAction) {
        return {conf: action.payload}
    },
});
