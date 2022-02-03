import {NotificationActions, NotificationMessage, NotificationType} from "../model/notification";
import {LoaderActions} from "../model/loader";

export async function handleResponseWithNotification(dispatch: Function,
                                                     okmessage: string,
                                                     notOkMessage: string,
                                                     fetchFunc: (() =>  Promise<Response>)): Promise<Response> {
    dispatch({type: LoaderActions.SHOW_LOADER})
    try {
        const response: Response = await fetchFunc();
        if (response.status == 200) {
            let message: NotificationMessage = {
                message: okmessage,
                type: NotificationType.SUCCESS
            }
            dispatch({type: NotificationActions.SHOW_NOTIFICATION, payload: message})
        } else if (response.status == 429) {
            let message: NotificationMessage = {
                message: "Server je zaneprázdněný. Zkus to znovu.",
                type: NotificationType.ERROR
            }
            dispatch({type: NotificationActions.SHOW_NOTIFICATION, payload: message})
        } else {
            let message: NotificationMessage = {
                message: notOkMessage,
                type: NotificationType.ERROR
            }
            dispatch({type: NotificationActions.SHOW_NOTIFICATION, payload: message})
        }
        return response;
    } catch (e) {
        let message: NotificationMessage = {
            message: "Chyba sítě. " + notOkMessage,
            type: NotificationType.ERROR
        }
        dispatch({type: NotificationActions.SHOW_NOTIFICATION, payload: message})
        throw e;
    } finally {
        dispatch({type: LoaderActions.HIDE_LOADER})
    }
}