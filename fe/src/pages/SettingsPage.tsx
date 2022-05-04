import React, {Dispatch, SetStateAction, useEffect} from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {getSettings, saveSettings} from "../actions/settings";
import {Settings} from "../model/settings";
import Grid from "@material-ui/core/Grid";
import {Button, Container, Input, InputLabel, Paper, Select} from "@material-ui/core";
import Switch from "@material-ui/core/Switch";
import {msToTime} from "./CurrentStatePage";
import WarningIcon from "@material-ui/icons/Warning";

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
    const recuperationEnabled = useSelector((state: RootState) => state.state.recuperationEnabled);
    const dispatch = useDispatch();
    const [values, setValues] = React.useState({...state.settings} as Settings);
    useEffect(() => {
        setValues(state.settings)
    }, [state])

    if (!values) {
        return <div>Loading..</div>
    }

    return <Container>
        <h1>Pokročilé nastavení</h1>
        <p>Pomocí následujících nastavení lze ladit zařízení pro konkrétní použití.
            Nutno zmínit, že jakákoliv změna provede automatický restart zařízení a tak dojde
            k ukončení činnost jakékoliv aktivního programu.
        </p>
        <Grid container>

            <Group>
                <h2>Rekuperační jednotka</h2>
                {!recuperationEnabled && <p>
                    <WarningIcon htmlColor={"orange"}/> Následující nastavení lze měnit pouze pokud je zapojená rekuperační jednotka.
                </p>}
                <SingleProperty values={values}
                          setValues={setValues}
                          disabled={!recuperationEnabled}
                          description={turnOffMotorCheck}
                          type="boolean"
                          title={"Kontrolovat otáčky Rekuperace"}
                          property={"checkRecuperationRpm"}/>
                <SingleProperty values={values}
                          description={maxRpm}
                            disabled={!recuperationEnabled}
                          setValues={setValues}
                          type="number"
                          title={"Maximální otáčky Rekuperace"}
                          property={"recuperationMaxRpm"}/>
                <SingleProperty values={values}
                          setValues={setValues}
                            disabled={!recuperationEnabled}
                          transform={Divide1000Transformation}
                          description={"Jak dlouho se má čekat na změnu směru otáčení. Nejméně 4 sekundy."}
                          type="number"
                          title={"Změna směru rekuperace"}
                          property={"recuperationWaitForDirectionChange"}/>
                <SingleProperty values={values}
                                setValues={setValues}
                                disabled={!recuperationEnabled}
                                transform={Divide1000Transformation}
                                description={"Jak dlouho trvá rekuperační cyklus. Nízká hodnota zvyšuje efektivitu tepelného výměníku, nicméně snižuje objem výměny vzduchu, neboť trvá relativně dlouho změnit směr motoru. Příliš vysoká hodnota vymění více vzduchu za cenu vyšších tepelných ztrát. Optimální hodnotu je třeba najít empiricky. Minimálně 40 sekund."}
                                type="number"
                                title={"Rekuperační cyklus"}
                                property={"recuperationCycleDuration"}/>
                <SingleProperty values={values}
                                setValues={setValues}
                                disabled={!recuperationEnabled}
                                description={"Počet revolucí pro jednu otáčku rekuperace (1-9). Lze najít v technické dokumentaci ventilátoru."}
                                type="number"
                                title={"Počet revolucí"}
                                property={"recuperationRevolutions"}/>
            </Group>
            <Group>
                <h2>Ventilační jednotka</h2>
                <SingleProperty values={values}
                          setValues={setValues}
                          description={turnOffMotorCheck}
                          type="boolean"
                          title={"Kontrolovat otáčky Ventilace"}
                          property={"checkVentilatorRpm"}/>
                <SingleProperty values={values}
                          setValues={setValues}
                          description={maxRpm}
                          type="number"
                          title={"Maximální otáčky Ventilace"}
                          property={"ventilatorMaxRpm"}/>
                <SingleProperty values={values}
                          setValues={setValues}
                          type="number"
                          title={"Maximální provozní teplota ventilátoru"}
                          description={"Toto nastavení pomáhá kontrolovat maximální provozní teplotu ventilátoru, aby nehrozilo případné přehřátí motoru. Je povolené tuto hodnotu překročit po dobu 1 minuty."}
                          property={"maxVentilatorTemp"}/>
                <SingleProperty values={values}
                                setValues={setValues}
                                description={"Počet revolucí pro jednu otáčku ventilace (1-9). Lze najít v technické dokumentaci ventilátoru."}
                                type="number"
                                title={"Počet revolucí"}
                                property={"ventilatorRevolutions"}/>
            </Group>


            <Group>
                <h2>Obecné</h2>
                <SingleProperty values={values}
                                setValues={setValues}
                                description={"Pokud je zapojená rekuperační jednotka, pak toto nastavení musí být zapnuté. Pokud není zapojená rekuperační jednotka, pak toto nastavení musí být vypnuté."}
                                type="boolean"
                                title={"Zapojená rekuperační jednotka"}
                                property={"recuperationOn"}/>
                <SingleProperty values={values}
                                setValues={setValues}
                                title={"Deaktivovaný program"}
                                transform={Divide1000Transformation}
                                description={"Doba trvání dočasně vypnutého programu, ve vteřinách."}
                                type="number"
                                property={"tempDisableDuration"}/>
                <SingleProperty values={values}
                                setValues={setValues}
                                transform={Divide1000Transformation}
                                type="number"
                                title={"Perioda odblokování ventilátorů"}
                                description={"Kolik sekund se má čekat pro další pokus roztočení motoru v případě registrace neočekávaného chování."}
                                property={"unblockingFansPeriod"}/>
            </Group>
            <Group>
                <h2>Výchozí nastavení párty módu</h2>
                {
                    recuperationEnabled && <><SingleProperty values={values} setValues={setValues}
                                property={"recuperationPower"}
                                title={"Výkon rekuperace"}
                                description={"Výkon rekuperační jednotky (od 0 do 100)."}
                                type={"number"}/><br/></>
                }
                <SingleProperty values={values} setValues={setValues}
                                title={"Výkon ventilace"}
                                property={"ventilatorPower"}
                                description={"Výkon ventilátoru (od 0 do 100)."}
                                type={"number"}/><br/>
                <SingleProperty values={values} setValues={setValues}
                                title={"Doba trvání"}
                                property={"durationMillis"}
                                transform={Divide1000Transformation}
                                description={"Doba trvání párty módu v sekundách, nejméně 60 vteřin."}
                                type={"number"}/>
            </Group>
            <Group>
                <h2>Soukromí</h2>
                <SingleProperty values={values}
                                setValues={setValues}
                                description={"Neposílat do monitorovacích služeb údaje o CO<sub>2</sub> čídlu."}
                                type="boolean"
                                title={"Schovat hodnoty oxidu uhličitého"}
                                property={"hideCo2"}/>
                <SingleProperty values={values}
                                setValues={setValues}
                                description={"Neposílat do monitorovacích služeb údaje o hodnotách vnitřní teploty a vlhkosti."}
                                type="boolean"
                                title={"Schovat vnitřní teplotu a vlhkost"}
                                property={"hideInternalTempHum"}/>
            </Group>
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
                <br/>
                <br/>
            </Grid>
        </Grid>
    </Container>
}

export interface PropertyProps {
    values: Settings
    title: string
    setValues: Dispatch<SetStateAction<Settings>>
    property: string
    description?: string
    type: "number" | "boolean"
    transform?: ValueTransformation
    disabled?: boolean
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
            <b>{toSentence(props.title)}</b><br/>
            <p>{props.description}</p>
        </InputLabel>
        {props.type === "boolean" &&
            <Switch
                disabled={props.disabled}
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
                disabled={props.disabled}
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
            padding: theme.spacing(2),
            marginBottom: theme.spacing(2),
            [theme.breakpoints.up("md")]: {
                margin: theme.spacing(2)
            },
        },
        label: {
        },
        item: {
            marginBottom: 10
        }
    }
})