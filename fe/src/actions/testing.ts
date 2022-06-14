import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export interface TestingData {
    tempOut?: number
    humOut?: number
    tempIn?: number
    humIn?: number
    co2?: number
}

export function setTestingSensors(data: TestingData) {
    return async function save(dispatch: Function, getState: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(data);
        let headers = new Headers({
            "Content-Type": "application/json"
        })

        await handleResponseWithNotification(dispatch,
            "Potvrzuji, sensor nastaven.",
            "Bohužel nebylo možné testing sensor nastavit.",
            () => {
            return fetch(baseurl + "/a/testing", {method: "POST", body: body, headers: headers});
        })
    }
}
