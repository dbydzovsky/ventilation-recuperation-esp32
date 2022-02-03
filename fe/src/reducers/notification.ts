import createReducer from "./createReducer";
import {NotificationAction, NotificationActions, NotificationState} from "../model/notification";


let initial: NotificationState = {notification: undefined}

export const notificationState = createReducer<NotificationState>(initial, {
    [NotificationActions.SHOW_NOTIFICATION](state: NotificationState, action: NotificationAction) {
        return {notification: action.payload}
    },
    [NotificationActions.CLEAR_NOTIFICATIONS](state: NotificationState, action: NotificationAction) {
        return {notification: undefined}
    },
});
