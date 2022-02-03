import {Configuration, ConfigurationActions} from "../model/configuration";
import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";


export function saveConfiguration(configuration: Configuration) {
    return async function save(dispatch: Function, getState: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(configuration);
        let headers = new Headers({
            "Content-Type": "application/json"
        })

        await handleResponseWithNotification(dispatch,
            "Konfigurace byla uložena",
            "Nebylo možné konfiguraci uložit",
            () => {
            return fetch(baseurl + "/a/conf/", {method: "POST", body: body, headers: headers});
        })
    }
}
export async function getConfiguration(dispatch: Function, getState: Function) {
    dispatch({type: LoaderActions.SHOW_LOADER})
    try {
        const response = await fetch(baseurl + "/a/conf/", {method: "GET"});
        const obj = await response.json();
        console.log(JSON.stringify(obj))
        dispatch({type: ConfigurationActions.GET_CONFIGURATION, payload: obj})
    } finally {
        dispatch({type: LoaderActions.HIDE_LOADER})
    }

}