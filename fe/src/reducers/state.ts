import createReducer from "./createReducer";
import {CurrentState, StateAction, StateActions} from "../model/state";
import {Mode} from "../model/configuration";

let initialState: CurrentState = {
    time: "",
    alive: 0,
    mode: Mode.INACTIVE,
    restarts: 0,
    insideTemp: {v: 0, avg: 0, err: 0, warn: 0},
    outsideTemp: {v: 0, avg: 0, err: 0, warn: 0},
    outsideHum: {v: 0, avg: 0, err: 0, warn: 0},
    co2Inside: {v: 0, avg: 0, err: 0, warn: 0},
    heap: 0,
    description: "",
    filterRecuperation: {
        remainingMinutes: 0,
        needCleaning: false,
    },
    filterVentilator: {
        remainingMinutes: 0,
        needCleaning: false,
    },
    alarmVentilator: {
        blocked: false,
        remainMinutes: 0,
        highRpm: false,
        needAttention: false
    },
    alarmRecuperation: {
        blocked: false,
        remainMinutes: 0,
        highRpm: false,
        needAttention: false
    },
    trial: {
        enabled: false,
        duration: 300000,
        mode: Mode.INACTIVE,
        ventilator: 0,
        recuperation: 0,
        recuperationMode: 0,
    },
    ventilator: 0,
    ventilatorRPM: 0,
    recuperation: 0,
    recuperationRPM: 0,
}

export const state = createReducer<CurrentState>(initialState, {
    [StateActions.GET_STATE](state: CurrentState, action: StateAction) {
        return action.payload
    },
});
