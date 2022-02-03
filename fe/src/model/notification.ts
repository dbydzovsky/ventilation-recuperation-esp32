
export enum NotificationActions{
  CLEAR_NOTIFICATIONS = "CLEAR_NOTIFICATION",
  SHOW_NOTIFICATION = "SHOW_NOTIFICATION"
}

export enum NotificationType {
  SUCCESS = "SUCCESS",
  INFO = "INFO",
  ERROR = "ERROR"
}

export interface NotificationMessage {
  type: NotificationType,
  message: string
}

export interface NotificationState {
  notification: NotificationMessage | undefined
}
interface NotificationActionType<T, P> {
  type: T;
  payload: P;
}

export type NotificationAction =
    | NotificationActionType<typeof NotificationActions.CLEAR_NOTIFICATIONS,void>
    | NotificationActionType<typeof NotificationActions.SHOW_NOTIFICATION, NotificationMessage>;
