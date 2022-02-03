import createReducer from "./createReducer";
import {ErrorActions} from "../model/error";
import {ConnectionState} from "../model/state";


export interface ErrorState {
    state: ConnectionState
}

let initialErrorState: ErrorState = {
    state: ConnectionState.Unitiliazed
}

export const errorState = createReducer<ErrorState>(initialErrorState, {
    [ErrorActions.ADD_NOT_CONNECTED](state: ErrorState) {
        if (state.state == ConnectionState.Disconnected) {
            return state;
        }
        return {...state, state: ConnectionState.Disconnected }
    },
    [ErrorActions.ADD_CONNECTED](state: ErrorState) {
        if (state.state == ConnectionState.Connected) {
            return state;
        }
        return {...state, state: ConnectionState.Connected }
    },
    [ErrorActions.ADD_TOO_MUCH_REQUESTS](state: ErrorState) {
        if (state.state == ConnectionState.TooMuchRequests) {
            return state;
        }
        return {...state, state: ConnectionState.TooMuchRequests }
    },
});
