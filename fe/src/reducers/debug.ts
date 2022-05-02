import createReducer from "./createReducer";
import {DebugActionType, DebugMessage, DebugVersions} from "../model/debug";
import {DebugActions} from "../model/debug";


export interface DebugState {
    messages: DebugMessage[]
    version: number
    appVersion: string
    trace: boolean
}
let initialDebugState: DebugState = {
    messages: [],
    version: -1,
    appVersion: "-1",
    trace: false,
}

export const debugState = createReducer<DebugState>(initialDebugState, {
    [DebugActions.SET_MESSAGES](state: DebugState, payload: DebugActionType<DebugMessage[], any>): DebugState {
        return { ...state, messages: payload.payload }
    },
    [DebugActions.SET_VERSION](state: DebugState, payload: DebugActionType<string, DebugVersions>): DebugState {
        return {
            ...state,
            version: payload.payload.version,
            appVersion: payload.payload.appVersion,
            trace: payload.payload.trace
        }
    },
});
