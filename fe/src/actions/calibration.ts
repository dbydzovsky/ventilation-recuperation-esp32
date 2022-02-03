import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export interface CalibrationData {
    insideTemp: number
}

export function calibrate(data: CalibrationData) {
    return async function save(dispatch: Function, getState: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(data);
        let headers = new Headers({
            "Content-Type": "application/json"
        })

        await handleResponseWithNotification(dispatch,
            "Zařízení bylo úspěšně kalibrováno",
            "Bohužel nebylo možné zařízení kalibrovat.",
            () => {
            return fetch(baseurl + "/a/cal/", {method: "POST", body: body, headers: headers});
        })
    }
}
