import React, {ChangeEvent, useEffect} from 'react'
import {makeStyles} from '@material-ui/core/styles';
import {
    Button,
    FormControl,
    FormControlLabel,
    FormLabel,
    Input,
    InputAdornment,
    InputLabel,
    Paper,
    Radio,
    RadioGroup
} from "@material-ui/core";
import {TurnOnOffRules} from "../components/TurnOnOffRules";
import {useActions} from "../actions";
import * as ConfigurationActions from "../actions/configuration";
import {getConfiguration} from "../actions/configuration";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {Configuration, Mode, Rule} from "../model/configuration";
import Grid from "@material-ui/core/Grid";
import PanToolIcon from '@material-ui/icons/PanTool';
import AcUnitIcon from '@material-ui/icons/AcUnit';
import PersonPinIcon from '@material-ui/icons/PersonPin';
import BrandingWatermarkIcon from '@material-ui/icons/BrandingWatermark';
import ExpansionPanel from '@material-ui/core/ExpansionPanel';
import ExpansionPanelSummary from '@material-ui/core/ExpansionPanelSummary';
import ExpansionPanelDetails from '@material-ui/core/ExpansionPanelDetails';
import Typography from '@material-ui/core/Typography';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import {Range} from "../components/Range";
import WarningIcon from "@material-ui/icons/Warning";
import WbSunnyIcon from '@material-ui/icons/WbSunny';
import CompareArrowsIcon from '@material-ui/icons/CompareArrows';

export const winterOrderValidation = (previous: Rule, actual: Rule) => {
    return previous.tv > (actual.tv - 3);
}
export const summerOrderValidation = (previous: Rule, actual: Rule) => {
    return previous.tv < (actual.tv + 3);
}
export const co2OrderValidation = (previous: Rule, actual: Rule) => {
    return previous.tv > (actual.tv - 50);
}
export const weatherKeys = [
    {key: "lat", label: "Zeměpisná šírka (např. 50.25)", max: 5},
    {key: "lon", label: "Zeměpisná délka (např. 15.39)", max: 5},
    {key: "weatherApiKey", label: "API klíč (max. 32 znaků)", max: 32}
]

export function co2Validator(input:string|number): boolean {
    if (input === "0") {
        return true
    } else {
        let candidate = Number(input)
        let valid = !Number.isNaN(candidate)
        if (valid) {
            return candidate > -1 && candidate < 5000
        }
        return false
    }
}
export function temperatureValidator(input: string | number) {
    if (input === "0") {
        return true
    } else {
        let candidate = Number(input)
        let valid = !Number.isNaN(candidate)
        if (valid) {
            return candidate > -20 && candidate < 100
        }
        return false
    }
}

export function rangeValidator(min: number, max: number) {
    return function (input: string | number) {
        let candidate = Number(input);
        if (Number.isNaN(candidate)) {
            return false
        }
        return candidate >= min && candidate <= max;
    }
}

export function lengthValidator(max: number) {
    return function (input: string | number) {
        let candidate = input + "";
        return candidate.length <= max;
    }
}

export function noopValidator() {
    return true
}

export function percentageValidator(input: string | number) {
    return rangeValidator(0, 100)(input)
}

function isASCII(str: string) {
    return /^[\x00-\x7F]*$/.test(str) && str.indexOf(" ") < 0;
}

export function ConfigurationPage() {
    const classes = useStyles();
    const configurationAction = useActions(ConfigurationActions);
    const currentState = useSelector((state: RootState) => state.state);
    const state = useSelector((state: RootState) => state.configuration);
    const recuperationEnabled = currentState.recuperationEnabled;
    const dispatch = useDispatch();
    const [reloadCounter, setReloadId] = React.useState(0);
    useEffect(() => {
        dispatch(getConfiguration);
    }, [reloadCounter]);
    let initial: Configuration = {
        autoSummerStart: 100,
        autoSummerEnd: 200,
        autoWinterStart: 250,
        autoWinterEnd: 50,
        name: "rekuperace",
        mode: 0,
        winterMaxInsideTemp: 24.0,
        summerMinInsideTemp: 18.0,
        minimumFeelsLike: 35,
        winterOnRules: [],
        summerOnRules: [],
        co2Rules: [],
        weatherApiKey: "",
        lat: "",
        lon: "",
        monitoring: {
            feed: "",
            key: ""
        }
    }
    const rulesMaxCount = 5;
    const [values, setValues] = React.useState({...state.conf} as Configuration);
    useEffect(() => {
        setValues(state.conf ? state.conf : initial as Configuration)
    }, [state])
    if (!values) {
        return <div></div>
    }
    const nameInvalid = !values.name || values.name.length > 20 || !isASCII(values.name);
    const maxInsideTemperatureInvalid = values.winterMaxInsideTemp < 0 || values.winterMaxInsideTemp > 1000;
    let winterRulesInvalid = values.winterOnRules && values.winterOnRules.some((item, index) => {
        return values.winterOnRules.filter((_, i) => {
            return i < index;
        }).some((previous) => winterOrderValidation(previous, item))
    })

    let summerRulesInvalid = values.summerOnRules && values.summerOnRules.some((item, index) => {
        return values.summerOnRules.filter((_, i) => {
            return i < index;
        })
            .some((previous) => summerOrderValidation(previous, item))
    })
    let co2RulesInvalid = values.co2Rules && values.co2Rules.some((item, index) => {
        return values.co2Rules.filter((_, i) => {
            return i < index;
        }).some((previous) => co2OrderValidation(previous, item))
    });
    let rulesInvalid = winterRulesInvalid
        || summerRulesInvalid
        || (values.winterOnRules && values.winterOnRules.length > rulesMaxCount)
        || (values.summerOnRules && values.summerOnRules.length > rulesMaxCount);
    if (recuperationEnabled) {
        rulesInvalid = rulesInvalid || co2RulesInvalid || (values.co2Rules && values.co2Rules.length > rulesMaxCount);
    }
    const isOverlapping = (a: number, b: number, c: number, d: number):boolean => {
        if (a < b && c < d) {
            return a < d && c < b;
        }
        if (a > b && c > d) {
            return true;
        }
        const m = 365;
        const w0 = (b - a) % m;
        const w1 = (d - c) % m;
        return (w1 != 0 && ((c - a) % m) < w0) || (w0 != 0 && ((a - c) % m) < w1);
    }
    const winterSummerOverlaps = isOverlapping(values.autoWinterStart, values.autoWinterEnd, values.autoSummerStart, values.autoSummerEnd)
    const anyInvalid = maxInsideTemperatureInvalid || rulesInvalid || nameInvalid || winterSummerOverlaps;
    const handleMonitoringChange = (prop: string) => (event: ChangeEvent<HTMLInputElement>) => {
        let monitoring = {...values.monitoring, [prop]: event.target.value}
        setValues({...values, monitoring: monitoring});
    };
    const handleChange = (prop: string, validator: (value: string) => boolean, transform: (value: string) => any = (it) => it) => (event: ChangeEvent<HTMLInputElement>) => {
        if (validator(event.target.value)) {
            setValues({...values, [prop]: transform(event.target.value)});
        } else {
            setValues({...values});
        }
    };

    const handleRuleChange = (prop: string) => (rules: Rule[]) => {
        setValues({...values, [prop]: rules});
    };

    const handleSave = () => {
        configurationAction.saveConfiguration(values)
        setReloadId(reloadCounter + 1)
    }
    const setMode = (e: ChangeEvent<HTMLInputElement>, value: string) => {
        setValues({...values, mode: Number(value)})
    }
    let monitoringUrl = "https://iotplotter.com/user/feed/";
    if (values.monitoring) {
        monitoringUrl += values.monitoring.feed;
    }

    return <div className={classes.header}>
        <Grid container>
            <Grid item xs={12} sm={6} md={6}>
                <Paper className={classes.properties}>
                    <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                        <BrandingWatermarkIcon/>
                        Název zařízení <i>(bude dostupné jako http://{values.name}/)</i>
                    </InputLabel>
                    <Input
                        error={nameInvalid}
                        id="standard-adornment-name"
                        value={values.name}
                        className={classes.input}
                        onChange={handleChange('name', (it) => it.length <= 20)}
                        startAdornment={<InputAdornment position="start"><PersonPinIcon/></InputAdornment>}
                    />
                </Paper>
            </Grid>
            <Grid item xs={12} md={12} lg={12}>
                <ExpansionPanel className={classes.griditem}>
                    <ExpansionPanelSummary
                        expandIcon={<ExpandMoreIcon/>}
                        aria-controls="panel1a-content"
                        id="panel1a-header"
                    >
                        <Typography>Nastavení monitoringu <i>(https://iotplotter.com)</i></Typography>
                    </ExpansionPanelSummary>
                    <ExpansionPanelDetails>
                        <Grid container>
                            <a target="_blank" href={monitoringUrl}>{monitoringUrl}</a>
                            Zařízení každou minutu odesílá hodnoty ze sensorů na webovou stránku iotplotter.com.
                            Využívá k tomu šifrovanou komunikaci.

                            Naměřené hodnoty pomáhají sledovat funkčnost zařízení, ale také sledovat jeho užitečnost.
                            Kromě hodnoty ze sensorů se odesílá také počet restartů, aktuální stav paměti a také počet
                            chybových čtení ze sensorů.
                            {[{key: "feed", label: "feed ID"}, {key: "key", label: "Api Key"}].map((item) => {
                                let value = ""
                                if (values.monitoring) {
                                    // @ts-ignore
                                    value = values.monitoring[item.key] || ""
                                }
                                return <Grid item xs={12} md={6} lg={4} key={item.key}>
                                    <Paper className={classes.properties}>
                                        <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                                            {item.label}
                                        </InputLabel>
                                        <Input
                                            id="standard-adornment-name"
                                            value={value}
                                            className={classes.input}
                                            onChange={handleMonitoringChange(item.key)}
                                            startAdornment={<InputAdornment
                                                position="start"><PersonPinIcon/></InputAdornment>}
                                        />
                                    </Paper>

                                </Grid>
                            })}
                        </Grid>

                    </ExpansionPanelDetails>
                </ExpansionPanel>
            </Grid>
            <Grid item xs={12} md={12} lg={12}>
                <ExpansionPanel className={classes.griditem}>
                    <ExpansionPanelSummary
                        expandIcon={<ExpandMoreIcon/>}
                        aria-controls="panel1a-content"
                        id="panel1a-header"
                    >
                        <Typography>Nastavení předpovědí <i>(https://openweathermap.org/)</i></Typography>
                    </ExpansionPanelSummary>
                    <ExpansionPanelDetails>
                        <Grid container>
                            {weatherKeys.map((item) => {
                                // @ts-ignore
                                let value = values[item.key] || ""
                                return <Grid item xs={12} md={6} lg={4} key={item.key}>
                                    <Paper className={classes.properties}>
                                        <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                                            {item.label}
                                        </InputLabel>
                                        <Input
                                            id="standard-adornment-name"
                                            value={value}
                                            className={classes.input}
                                            onChange={handleChange(item.key, lengthValidator(item.max))}
                                            startAdornment={<InputAdornment
                                                position="start"><PersonPinIcon/></InputAdornment>}
                                        />
                                    </Paper>

                                </Grid>
                            })}
                        </Grid>

                    </ExpansionPanelDetails>
                </ExpansionPanel>
            </Grid>
            <Grid item xs={12} sm={12} md={12}>
                <Paper className={classes.properties} key={values.mode + ""}>
                    <FormControl component="fieldset">
                        <div className={classes.label}>
                            <FormLabel component="legend"><CompareArrowsIcon/> Režim provozu</FormLabel>
                        </div>
                        <RadioGroup aria-label="mode" name="gender1" value={values.mode}
                                    onChange={setMode} className={classes.radioBtnGroup}>
                            <FormControlLabel value={Mode.INACTIVE}
                                              control={<Radio/>}
                                              className={values.mode == Mode.INACTIVE ? classes.selectedFont : undefined}
                                              label="Neaktivní"/>
                            <FormControlLabel value={Mode.WINTER}
                                              control={<Radio/>}
                                              className={values.mode == Mode.WINTER ? classes.selectedFont : undefined}
                                              label="Ohřívání vzduchu a topení"/>
                            <FormControlLabel value={Mode.SUMMER}
                                              control={<Radio/>}
                                              className={values.mode == Mode.SUMMER ? classes.selectedFont : undefined}
                                              label={<>Noční chlazení <i>(Zařízení musí být připojené k WiFi)</i></>}/>
                            <FormControlLabel value={Mode.AUTO}
                                              control={<Radio/>}
                                              className={values.mode == Mode.AUTO ? classes.selectedFont : undefined}
                                              label={<>Automaticky <i>(Zařízení musí být připojené k WiFi, odkud si
                                                  synchronizuje čas a předpověď počasí)</i></>}/>
                        </RadioGroup>
                    </FormControl>
                </Paper>
            </Grid>
        </Grid>
        <div className={classes.winterSetting}>
            <ExpansionPanel className={classes.griditem}>
                <ExpansionPanelSummary
                    expandIcon={<ExpandMoreIcon/>}
                    aria-controls="panel1a-content"
                    id="panel1a-header"
                >
                    <h2><AcUnitIcon/> Nastavení vyhřívacího programu</h2>
                </ExpansionPanelSummary>
                <ExpansionPanelDetails>
                    <Grid container>
                        <Grid item xs={12} sm={12} md={12}>

                            <p>Vyhřívací program je vhodný na zimní období, neboť při zvýšené teplotě v kolektoru
                                dojde k sepnutí ventilátoru dle následujícího nastavení pravidel. Lze nastavit až 5
                                pravidel. Lze také nastavit vnitřní teplotu, při které se má vytápění deaktivovat, což
                                slouží k prevenci přehřátí místnosti (Pro deaktivaci tohoto nastavení lze nastavit např. 40 stupnu)</p>
                        </Grid>
                        <Grid item xs={12} sm={6} md={4}>
                            <Paper className={classes.properties}>
                                <InputLabel className={classes.label}>
                                    <PanToolIcon/>
                                    Deaktivovat při vnitřní teplotě větší než:
                                </InputLabel>
                                <Input
                                    error={maxInsideTemperatureInvalid}
                                    id="standard-adornment-max-inside-temp"
                                    type={"number"}
                                    value={values.winterMaxInsideTemp}
                                    className={classes.input}
                                    onChange={handleChange('winterMaxInsideTemp', temperatureValidator, (val: string) => {
                                        console.log(val);
                                        return Math.round(Number(val) * 100) / 100
                                    })}
                                    startAdornment={<InputAdornment position="start">°C</InputAdornment>}
                                />
                            </Paper>
                        </Grid>
                        <Grid item xs={12} sm={12} md={12}>
                            <TurnOnOffRules header={"Pravidla pro sepnutí (max 5, od nejmenší vnější teploty po největší):"}
                                            adornment={"°C"}
                                            placeholder={"Teplota"}
                                            requiredDifference={3}
                                            newDefault={20}
                                            valueValidator={temperatureValidator}
                                            rules={values.winterOnRules}
                                            validate={winterOrderValidation}
                                            onChange={handleRuleChange('winterOnRules')}/>
                        </Grid>
                    </Grid>
                </ExpansionPanelDetails>
            </ExpansionPanel>

        </div>
        <div className={classes.summerSetting}>
            <ExpansionPanel className={classes.griditem}>
                <ExpansionPanelSummary
                    expandIcon={<ExpandMoreIcon/>}
                    aria-controls="panel1a-content"
                    id="panel1a-header"
                >
                    <h2><WbSunnyIcon/> Nastavení chladícího programu</h2>
                </ExpansionPanelSummary>
                <ExpansionPanelDetails>
                    <Grid container>
                        <Grid item xs={12} sm={12} md={12}>

                            <p>Chladící program je vhodný na letní období, kdy během chladnějších nocí vychladí místnost na
                                určenou teplotu. Pro tento program je nutné připojit zařízení k internetu prostřednictvím WiFi.
                                Z internetu získává data o předpovědi počasí. Předpověď počasí se používá pro řízení
                                chlazení, neboť dává smysl chladit místnost pouze v okamžiku, když bude další den
                                pocitová teplota větší, než nastavená.</p>
                            <p>Nutno zmínit, že s ohledem na bezpečnost dochází ke chlazení místnosti pouze pokud
                                nehrozí dle výpočtu rosného bodu vznik kondenzace.</p>
                        </Grid>
                        <Grid item xs={12} sm={6} md={4}>
                            <Paper className={classes.properties}>
                                <InputLabel className={classes.label}>
                                    <PanToolIcon/>
                                    Deaktivovat při teplotě menší než::
                                </InputLabel>
                                <Input
                                    error={maxInsideTemperatureInvalid}
                                    id="standard-adornment-min-inside-temp"
                                    type={"number"}
                                    value={values.summerMinInsideTemp}
                                    className={classes.input}
                                    onChange={handleChange('summerMinInsideTemp', temperatureValidator, (val: string) => {
                                        console.log(val);
                                        return Math.round(Number(val) * 100) / 100
                                    })}
                                    startAdornment={<InputAdornment position="start">°C</InputAdornment>}
                                />

                            </Paper>
                        </Grid>
                        <Grid item xs={12} sm={6} md={4}>
                            <Paper className={classes.properties}>
                                <InputLabel className={classes.label}>
                                    <WbSunnyIcon/>
                                    Zapnout chlazení pokud je pocitová teplota (podle předpovědi) větší než:
                                </InputLabel>
                                <Input
                                    error={maxInsideTemperatureInvalid}
                                    id="standard-adornment-minimumFeelsLike"
                                    type={"number"}
                                    value={values.minimumFeelsLike}
                                    className={classes.input}
                                    onChange={handleChange('minimumFeelsLike', temperatureValidator, (val: string) => {
                                        console.log(val);
                                        return Math.round(Number(val) * 100) / 100
                                    })}
                                    startAdornment={<InputAdornment position="start">°C</InputAdornment>}
                                />

                            </Paper>
                        </Grid>

                        <Grid item xs={12} sm={12} md={12}>
                            <TurnOnOffRules header={"Pravidla pro sepnutí (max 5, od nejvyšší vnější teploty po nejmenší):"}
                                            adornment={"°C"}
                                            placeholder={"Teplota"}
                                            newDefault={20}
                                            requiredDifference={3}
                                            valueValidator={temperatureValidator}
                                            rules={values.summerOnRules}
                                            validate={summerOrderValidation}
                                            onChange={handleRuleChange('summerOnRules')}/>
                        </Grid>
                    </Grid>
                </ExpansionPanelDetails>
            </ExpansionPanel>

        </div>
        { recuperationEnabled && <div className={classes.co2Setting}>
            <ExpansionPanel className={classes.griditem}>
                <ExpansionPanelSummary
                    expandIcon={<ExpandMoreIcon/>}
                    aria-controls="panel1a-content"
                    id="panel1a-header"
                >
                    <h2><CompareArrowsIcon/> Nastavení řízeného větrání</h2>
                </ExpansionPanelSummary>
                <ExpansionPanelDetails>
                    <Grid container>
                        <Grid item xs={12} sm={12} md={12}>

                            <p>
                                V přirozeném prostředí je koncentrace CO2 kolem 400 ppm (parts per million, tj. počet částic na milion),
                                v průmyslových oblastech je hodnota o něco vyšší.
                                Přibližně 20% populace začíná negativně reagovat již při koncentraci CO2 kolem 1000 ppm.
                                Pro regulační systémy bývá často tato hodnota nastavena jako limitní pro ovládání výkonu vzduchotechnických
                                jednotek nebo výměny vzduchu v místnosti.
                            </p>
                        </Grid>
                        <Grid item xs={12} sm={12} md={12}>
                            <TurnOnOffRules header={"Pravidla pro sepnutí rekuperace (max 5, od nejnižší hodnoty PPM):"}
                                            adornment={"PPM"}
                                            placeholder={"Co2"}
                                            newDefault={800}
                                            requiredDifference={50}
                                            valueValidator={co2Validator}
                                            rules={values.co2Rules}
                                            validate={co2OrderValidation}
                                            onChange={handleRuleChange('co2Rules')}/>
                        </Grid>
                    </Grid>
                </ExpansionPanelDetails>
            </ExpansionPanel>

        </div> }
        <div className={classes.autoSetting}>
            <ExpansionPanel className={classes.griditem}>
                <ExpansionPanelSummary
                    expandIcon={<ExpandMoreIcon/>}
                    aria-controls="panel1a-content"
                    id="panel1a-header"
                >
                    <h2>Nastavení automatického programu</h2>
                </ExpansionPanelSummary>
                <ExpansionPanelDetails>
                    <Grid container>
                        <Grid item xs={12} sm={12} md={12}>

                            <p>
                                Pro aktivaci automatického programu je třeba zařízení udržovat připojené k internetu.
                                V opačném případě nebude zařízení schopné určit aktuální datum
                                a nebude tak vědět, který program si má vybrat.
                                Automatický program usnadňuje správu ventilační jednotky, neboť
                                automaticky přepíná režimy dle kalendáře.
                            </p>
                        </Grid>
                        <Grid item xs={12} sm={12} md={12}>
                            <Paper className={classes.properties}>
                                Nastavení období vyhřívacího programu
                                <Range color="orange" start={values.autoWinterStart} end={values.autoWinterEnd}
                                       onChange={(start, end) => {
                                           setValues({...values, autoWinterStart: start, autoWinterEnd: end})
                                       }}/>
                            </Paper>
                        </Grid>
                        <Grid item xs={12} sm={12} md={12}>
                            <Paper className={classes.properties}>
                                Nastavení období klimatizačního programu
                                <Range color="blue" start={values.autoSummerStart} end={values.autoSummerEnd}
                                       onChange={(start, end) => {
                                           setValues({...values, autoSummerStart: start, autoSummerEnd: end})
                                       }}/>
                            </Paper>
                        </Grid>
                        {winterSummerOverlaps ? <div className={classes.warn}>
                            <WarningIcon/> Obě dvě období se nesmí prolínat
                        </div> : ""}
                    </Grid>
                </ExpansionPanelDetails>
            </ExpansionPanel>
            </div>
        <Button onClick={handleSave} disabled={anyInvalid} color={"secondary"} variant={"contained"}
                className={classes.btn}>Uložit</Button>
        <div className={classes.nothing}></div>
    </div>
}


const useStyles = makeStyles({
    label: {
        display: "flex",
        alignItems: "center",
    },
    helplabel: {
        fontSize: "80%",
        fontStyle: "italic"
    },
    radioBtnGroup: {
        display: "block",
    },
    griditem: {
        margin: 20,
    },
    winterSetting: {
        borderLeft: "red 2px solid",
        marginTop: 10
    },
    autoSetting: {
        borderLeft: "orange 2px solid",
        marginTop: 10,
    },
    summerSetting: {
        borderLeft: "blue 2px solid",
        marginTop: 10,
    },
    co2Setting: {
        borderLeft: "violet 2px solid",
        marginTop: 10,
    },
    selectedFont: {
        fontWeight: "bold",
        color: "red",
    },
    properties: {
        margin: 20,
        padding: 20
    },
    header: {},
    input: {
        margin: 20
    },
    btn: {
        margin: 20
    },
    nothing: {
        margin: 50
    },
    warn: {
        color: "orange"
    }
})