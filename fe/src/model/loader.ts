export enum LoaderActions{
  SHOW_LOADER = "SHOW_LOADER",
  HIDE_LOADER = "HIDE_LOADER",
}

export interface LoaderState {
  loading: boolean
}

interface LoaderActionType<T, P> {
  type: T;
  payload: P;
}

export type StateAction =
    | LoaderActionType<typeof LoaderActions.HIDE_LOADER, void>
    | LoaderActionType<typeof LoaderActions.SHOW_LOADER, void>;
