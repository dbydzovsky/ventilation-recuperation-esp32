import {StateActions} from "../model/state";
import {ErrorActions} from "../model/error";
import {fetchWithTimeout} from "../util/timeout";
import {ClearSettingsActions, SettingsActions} from "../model/settings";
import {Error} from "@material-ui/icons";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export function getState(props: { onDone: () => void}) {
    return async function save(dispatch: Function, getState: Function) {
        try {
            const response = await fetchWithTimeout(baseurl + "/a/s/", {
                timeout: 2000,
                method: "GET"
            });
            if (response.status == 429) {
                dispatch({type: ErrorActions.ADD_TOO_MUCH_REQUESTS})
                dispatch({type: SettingsActions.CLEAR_SETTINGS, payload: {connected: true}} as ClearSettingsActions)
            } else {
                const obj = await response.json();
                dispatch({type: StateActions.GET_STATE, payload: obj})
                dispatch({type: ErrorActions.ADD_CONNECTED})
                dispatch({type: SettingsActions.CLEAR_SETTINGS, payload: {connected: true}} as ClearSettingsActions)
            }
            props.onDone();
        } catch (e) {
            dispatch({type: ErrorActions.ADD_NOT_CONNECTED})
            dispatch({type: SettingsActions.CLEAR_SETTINGS, payload: {connected: false}} as ClearSettingsActions)
            props.onDone();
        }
    }
}