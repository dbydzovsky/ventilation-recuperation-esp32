import createReducer from "./createReducer";
import {CurrentState, StateAction, StateActions} from "../model/state";
import {Mode} from "../model/configuration";

let initialState: CurrentState = {
    t: "",
    a: 0,
    mo: Mode.INACTIVE,
    p: 0,
    r: 0,
    i: {t: 0, tAvg: 0,tErr: 0,tWarn: 0},
    o: {t: 0, h: 0,tAvg: 0,tErr: 0,tWarn: 0,  hAvg: 0,hErr: 0, hWarn: 0},
    h: 0,
    de: ""
}

export const state = createReducer<CurrentState>(initialState, {
    [StateActions.GET_STATE](state: CurrentState, action: StateAction) {
        return action.payload
    },
});
