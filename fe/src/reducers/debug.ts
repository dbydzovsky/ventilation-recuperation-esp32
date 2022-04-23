import createReducer from "./createReducer";
import {DebugActionType, DebugMessage} from "../model/debug";
import {DebugActions} from "../model/debug";


export interface DebugState {
    messages: DebugMessage[]
    version: number
}

let initialDebugState: DebugState = {
    messages: [],
    version: -1
}

export const debugState = createReducer<DebugState>(initialDebugState, {
    [DebugActions.SET_MESSAGES](state: DebugState, payload: DebugActionType<DebugMessage[], any>): DebugState {
        return { ...state, messages: payload.payload }
    },
    [DebugActions.SET_VERSION](state: DebugState, payload: DebugActionType<string, any>): DebugState {
        return { ...state, version: payload.payload }
    },
});
