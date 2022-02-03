import {StateActions} from "../model/state";
import {ErrorActions} from "../model/error";
import {fetchWithTimeout} from "../util/timeout";

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
            } else {
                const obj = await response.json();
                dispatch({type: StateActions.GET_STATE, payload: obj})
                dispatch({type: ErrorActions.ADD_CONNECTED})
            }
            props.onDone();
        } catch (e) {
            dispatch({type: ErrorActions.ADD_NOT_CONNECTED})
            props.onDone();
        }
    }
}