export enum ErrorActions{
  ADD_TOO_MUCH_REQUESTS = "ADD_TOO_MUCH_REQUESTS",
  ADD_NOT_CONNECTED = "ADD_NOT_CONNECTED",
  ADD_CONNECTED = "ADD_CONNECTED",
}

interface ErrorActionType<T, P> {
  type: T;
  payload: P;
}

export type ErrorAction =
    | ErrorActionType<typeof ErrorActions.ADD_TOO_MUCH_REQUESTS , void>
    | ErrorActionType<typeof ErrorActions.ADD_NOT_CONNECTED, void>
    | ErrorActionType<typeof ErrorActions.ADD_CONNECTED, void>;
