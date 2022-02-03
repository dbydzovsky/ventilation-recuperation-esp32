export enum ConfigurationActions{
  GET_CONFIGURATION = "GET_CONFIGURATION",
}

export interface Rule {
  t: number
  p: number
}

export interface Monitoring {
  f: string,
  k: string
}

export enum Mode {
  INACTIVE = 0,
  WINTER = 1,
  SUMMER = 2,
  AUTO = 3,
}

export interface Configuration {
  c: number
  n: string
  mo: Mode
  td: number // treding down
  wmit: number
  wor: Rule[]
  sor: Rule[]
  smit: number
  m: Monitoring
  w: string
  lat: string
  lon: string
  aws: number
  awe: number
  ass: number
  ase: number
}

export interface ConfigurationState {
  conf: Configuration | undefined
}

interface ConfigurationActionType<T, P> {
  type: T;
  payload: P;
}

export type ConfigurationAction =
    | ConfigurationActionType<typeof ConfigurationActions.GET_CONFIGURATION, Configuration>;