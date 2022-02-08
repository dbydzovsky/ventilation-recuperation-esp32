import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";
import {FanType} from "./filter";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export interface CleanAlarmData {
    filter: FanType
}

export function cleanAlarm(data: CleanAlarmData) {
    return async function save(dispatch: Function, getState: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(data);
        let headers = new Headers({
            "Content-Type": "application/json"
        })

        await handleResponseWithNotification(dispatch,
            "Potvrzuji, alarm byl restartován.",
            "Bohužel nebylo možné označit alarm uvolnit.",
            () => {
            return fetch(baseurl + "/a/alarm/", {method: "POST", body: body, headers: headers});
        })
    }
}
