import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";
import {getState} from "./state";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export enum RecuperationMode {
    Recycle = 1,
    Inhale = 2,
    Exhale = 3
}

export interface ChangeTrialProps {
    ventilator: number,
    recuperation: number,
    recuperationMode: RecuperationMode,
    on: boolean,
    duration: number
}

export function changeTrial(props: ChangeTrialProps){
    return async function save(dispatch: Function, _: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(props);
        let headers = new Headers({
            "Content-Type": "application/json"
        })
        await handleResponseWithNotification(dispatch,
            props.on ? "Výkon upraven." : "Úspěšně deaktivováno.",
            props.on ? "Výkon nebylo možné dočasně upravit" : "Nepovedlo se deaktivovat.",
            () => {
                return fetch(baseurl + "/a/t/", {method: "POST", body: body, headers: headers}).then((x) => {
                    if (x.status >= 200 && x.status < 300) {
                        dispatch(getState({onDone: () => {}}))
                    }
                    return x
                });
            })
    }
}