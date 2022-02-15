export enum SettingsActions{
  GET_SETTINGS = "GET_SETTINGS",
}

export interface Settings {
  checkRecuperationRpm: boolean
  checkVentilatorRpm: boolean
  unblockingFansPeriod: number
  ventilatorMaxRpm: number
  recuperationMaxRpm: number
  syncForecastTolerateLastSuccessFor: number
  syncForecastInterval: number
  relayCooldown: number
  recuperationWaitForDirectionChange: number
  recuperationCycleDuration: number
}

export interface SettingsState {
  settings: Settings
}

interface SettingsActionType<T, P> {
  type: T;
  payload: P;
}

export type SettingsAction =
    | SettingsActionType<typeof SettingsActions.GET_SETTINGS, Settings>;