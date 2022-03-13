import React, {Dispatch, SetStateAction, useEffect} from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {getSettings, saveSettings} from "../actions/settings";
import {Settings} from "../model/settings";
import Grid from "@material-ui/core/Grid";
import {Button, Input, InputLabel, Paper, Select} from "@material-ui/core";
import Switch from "@material-ui/core/Switch";
import {msToTime} from "./CurrentStatePage";

export interface Props {

}
const turnOffMotorCheck = "Zde je možné vypnout kontrolu otáček motoru. Je doporučeno toto nastavení nechat zapnuté. Motor se vypne v případě příliš vysokých otáček v případě vichřice, popř. vypne, pokud je porouchaný sensor otáček."
const maxRpm = "Maximální povolený počet otáček za minutu pro motor. V případě příliš nízké hodnoty se může motor často blokovat, v případě příliš vysoké hodnoty hrozí poškození motoru v případě nevypnutí motoru při vysokých otáčkách."

export interface ValueTransformation {
    fromGui: (val: number) => number
    toGui: (val: number) => number
}
const Divide1000Transformation: ValueTransformation = {
    fromGui: (x) => x * 1000,
    toGui: (x) => x / 1000
}
const EmptyTransformation: ValueTransformation = {
    toGui: x => x,
    fromGui: x => x
}
export function SettingsLoader() {
    const errorState = useSelector((state: RootState) => state.errorState);
    const dispatch = useDispatch();
    useEffect(() => {
        dispatch(getSettings);
    }, [errorState.state]);
    return null
}
export function SettingsPage(props: Props) {
    const classes = useStyles()
    const state = useSelector((state: RootState) => state.settings);
    const dispatch = useDispatch();
    const [values, setValues] = React.useState({...state.settings} as Settings);
    useEffect(() => {
        setValues(state.settings)
    }, [state])

    if (!values) {
        return <div>Loading..</div>
    }

    return <div className={classes.paper}>
        <h1>Pokročilé nastavení</h1>
        <Grid container>
            <Property values={values}
                      setValues={setValues}
                      description={turnOffMotorCheck}
                      type="boolean"
                      property={"checkRecuperationRpm"}/>
            <Property values={values}
                      setValues={setValues}
                      description={turnOffMotorCheck}
                      type="boolean"
                      property={"checkVentilatorRpm"}/>
            <Property values={values}
                      setValues={setValues}
                      transform={Divide1000Transformation}
                      type="number"
                      description={"Kolik sekund se má čekat pro další pokus roztočení motoru v případě registrace neočekávaného chování."}
                      property={"unblockingFansPeriod"}/>
            <Property values={values}
                      setValues={setValues}
                      description={maxRpm}
                      type="number"
                      property={"ventilatorMaxRpm"}/>
            <Property values={values}
                      description={maxRpm}
                      setValues={setValues}
                      type="number"
                      property={"recuperationMaxRpm"}/>
            <Property values={values}
                      setValues={setValues}
                      transform={Divide1000Transformation}
                      description={"Jak dlouho se má čekat na změnu směru otáčení. Nejméně 4 sekundy."}
                      type="number"
                      property={"recuperationWaitForDirectionChange"}/>
            <Property values={values}
                      setValues={setValues}
                      transform={Divide1000Transformation}
                      description={"Jak dlouho trvá rekuperační cyklus. Nízká hodnota zvyšuje efektivitu tepelného výměníku, nicméně snižuje objem výměny vzduchu, neboť trvá relativně dlouho změnit směr motoru. Příliš vysoká hodnota vymění více vzduchu za cenu vyšších tepelných ztrát. Optimální hodnotu je třeba najít empiricky. Minimálně 40 sekund."}
                      type="number"
                      property={"recuperationCycleDuration"}/>
            <Group>
                <h2>Defaultní nastavení párty módu</h2>
                <SingleProperty values={values} setValues={setValues}
                                property={"recuperationPower"}
                                description={"Výkon rekuperační jednotky (od 0 do 100)."}
                                type={"number"}/><br/>
                <SingleProperty values={values} setValues={setValues}
                                property={"ventilatorPower"}
                                description={"Výkon ventilátoru (od 0 do 100)."}
                                type={"number"}/><br/>
                <SingleProperty values={values} setValues={setValues}
                                property={"durationMillis"}
                                transform={Divide1000Transformation}
                                description={"Doba trvání párty módu v sekundách, nejméně 60 vteřin."}
                                type={"number"}/>
            </Group>
            <Property values={values}
                      setValues={setValues}
                      transform={Divide1000Transformation}
                      description={"Doba trvání dočasně vypnutého programu, ve vteřinách."}
                      type="number"
                      property={"tempDisableDuration"}/>
            {/*<Property values={values}*/}
            {/*          setValues={setValues}*/}
            {/*          description={"Intenzita LED v procentech (0-100)."}*/}
            {/*          type="number"*/}
            {/*          property={"brightness"}/>*/}
            <Grid item xs={12}>
                <Button variant={"contained"} color={"secondary"} onClick={() => {
                    dispatch(saveSettings(values))
                }}>
                    Uložit nové nastavení
                </Button>
            </Grid>
        </Grid>
    </div>
}

export interface PropertyProps {
    values: Settings
    setValues: Dispatch<SetStateAction<Settings>>
    property: string
    description?: string
    type: "number" | "boolean"
    transform?: ValueTransformation
}


function toSentence(text: string): string {
    const result = text.replace(/([A-Z])/g, " $1");
    return result.charAt(0).toUpperCase() + result.slice(1);
}

export function Group(props: {children: any | any[]}) {
    const classes = useStyles()
    return <Grid item xs={12} md={6}>
        <Paper className={classes.paper}>
            {props.children}
        </Paper>
    </Grid>
}

function SingleProperty(props: PropertyProps) {
    const classes = useStyles()
    const id = "standard-adornment-" + props.property
    let transformation = EmptyTransformation
    if (props.transform) {
        transformation = props.transform
    }
    return <div className={classes.item}>
        <InputLabel htmlFor={id} className={classes.label}>
            <b>{toSentence(props.property)}</b><br/>
            <p>{props.description}</p>
        </InputLabel>
        {props.type == "boolean" &&
            <Switch
                checked={(props.values as any)[props.property] as boolean}
                onChange={(e, newValue) => {
                    props.setValues({...props.values, [props.property]: Boolean(newValue)})
                }}
                id={id}
                color="secondary"
                name={props.property}
                inputProps={{'aria-label': 'secondary checkbox'}}
            />
        }
        {props.type == "number" &&
            <Input
                id={id}
                value={transformation.toGui((props.values as any)[props.property] as number)}
                onChange={(e) => {
                    let value: any = e.target.value
                    // if (props.type == "number") {
                    //     value = Number(value)
                    // } else if (props.type=="boolean") {
                    //     value = Boolean(value)
                    // }
                    props.setValues({...props.values, [props.property]: transformation.fromGui(value)})
                }}
                placeholder={props.property}
            />
        }
    </div>
}

export function Property(props: PropertyProps) {
    return <Group>
            <SingleProperty {...props}/>
        </Group>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        paper: {
            margin: 20,
            padding: 20
        },
        label: {
        },
        item: {
            marginBottom: 10
        }
    }
})