import createReducer from "./createReducer";
import {DebugActionType, DebugMessage} from "../model/debug";
import {DebugActions} from "../model/debug";


export interface DebugState {
    messages: DebugMessage[]
}

let initialDebugState: DebugState = {
    messages: []
}

export const debugState = createReducer<DebugState>(initialDebugState, {
    [DebugActions.SET_MESSAGES](state: DebugState, payload: DebugActionType<DebugMessage[], any>): DebugState {
        return { messages: payload.payload }
    },
});
