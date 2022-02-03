import createReducer from "./createReducer";
import {LoaderActions, LoaderState} from "../model/loader";


let loaderState: LoaderState = {
    loading: false
}

export const loader = createReducer<LoaderState>(loaderState, {
    [LoaderActions.SHOW_LOADER](state: LoaderState) {
        return {loading: true}
    },
    [LoaderActions.HIDE_LOADER](state: LoaderState) {
        return {loading: false}
    },
});
