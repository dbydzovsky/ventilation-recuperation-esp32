import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export enum Filter {
    ventilator = 1,
    recuperation = 2,
}

export interface CleanFilterData {
    filter: Filter
}

export function cleanFilter(data: CleanFilterData) {
    return async function save(dispatch: Function, getState: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(data);
        let headers = new Headers({
            "Content-Type": "application/json"
        })

        await handleResponseWithNotification(dispatch,
            "Potvrzuji, filter vyčištěň.",
            "Bohužel nebylo možné označit filter za vyčištěný.",
            () => {
            return fetch(baseurl + "/a/clean/", {method: "POST", body: body, headers: headers});
        })
    }
}
