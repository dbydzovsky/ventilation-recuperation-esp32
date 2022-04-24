import React from 'react'
import FiberManualRecordIcon from '@material-ui/icons/FiberManualRecord';
import LooksOneIcon from '@material-ui/icons/LooksOne';
import Looks3Icon from '@material-ui/icons/Looks3';
import LooksTwo from '@material-ui/icons/LooksTwo';
import TimerIcon from '@material-ui/icons/Timer';
import Grid from "@material-ui/core/Grid";
import {makeStyles} from "@material-ui/styles";
import {Theme} from "@material-ui/core/styles";
import GitHubIcon from "@material-ui/icons/GitHub";
import {InstallationPage} from "./InstallationPage";
import ExpansionPanelSummary from "@material-ui/core/ExpansionPanelSummary";
import ExpandMoreIcon from "@material-ui/icons/ExpandMore";
import Typography from "@material-ui/core/Typography";
import ExpansionPanelDetails from "@material-ui/core/ExpansionPanelDetails";
import {Input, InputAdornment, InputLabel, Paper} from "@material-ui/core";
import PersonPinIcon from "@material-ui/icons/PersonPin";
import ExpansionPanel from "@material-ui/core/ExpansionPanel";

enum SignalDiode {
    Red = "red",
    Green = "green",
    Orange = "orange",
    None = "none"
}

const Red = SignalDiode.Red;
const Green = SignalDiode.Green;
const Orange = SignalDiode.Orange;
const None = SignalDiode.None;

function DiodeSignalization(props: { data: (SignalDiode)[] }) {
    if (props.data.length != 12) {
        throw new Error("Must be 12 in size.")
    }
    return <>
        {props.data.map(signal => {
            let color = "lightgrey";
            if (signal == SignalDiode.Red) {
                color = "red";
            } else if (signal == SignalDiode.Green) {
                color = "green"
            } else if (signal == SignalDiode.Orange) {
                color = "orange"
            }
            return <FiberManualRecordIcon htmlColor={color}/>
        })}
    </>
}

const TrialSignal = [Green, None, Orange, None, Green, None, Orange, None, Green, None, Orange, None];
const WiFiConnectingSignal = [Green, Orange, Red, Green, Orange, Red, Green, Orange, Red, Green, Orange, Red];
const WinterDeviceOperationalWifiOnSignal = [None, None, None, None, None, None, None, None, None, Orange, None, Orange];
const WinterDeviceOperationalWifiOffSignal = [None, None, None, None, None, None, None, None, None, None, None, Orange];
const SummerDeviceOperationalWifiOnSignal = [None, None, None, None, None, None, None, None, None, Green, None, Green];
const SummerDeviceOperationalWifiOffSignal = [None, None, None, None, None, None, None, None, None, None, None, Green];
const DeviceDisabledWifiOnSignal = [None, None, None, None, None, None, None, None, None, Red, None, Red];
const DeviceDisabledWifiOffSignal = [None, None, None, None, None, None, None, None, None, None, None, Red];
const WarmingUpSignal = [Orange, Orange, None, Orange, None, Orange, Orange, None, Orange, None, Orange, Orange];
const NotValidSensorValues = [Red, None, Red, None, Red, None, Red, None, Red, None, Red, None];
const DisabledDueToConditions = [None, None, None, None, None, None, None, None, Green, Green, Orange, Orange];
const CriticalSignal = [Red, Red, Red, Red, None, None, None, None, Red, Red, Red, Red];

export function Row(props: { label: any, value: any }) {
    const classes = useStyles();
    return <Grid item xs={12}>
        <Grid container className={classes.row}>
            <Grid item className={classes.flex} xs={12} sm={12} md={6} lg={4}>
                {props.label}
            </Grid>
            <Grid item className={classes.flex} xs={12} sm={12} md={6} lg={8}>
                {props.value}
            </Grid>
        </Grid>
    </Grid>
}

export function HelpPage() {
    const classes = useStyles();
    return <div className={classes.root}>
        <div className={classes.center}>
            <h1>Nápověda - ovládání rekuperace</h1>
        </div>
        {/*<h2 className={classes.header}>Display</h2>*/}
        {/*<h2 className={classes.header}>Význam světelné signalizace</h2>*/}
        {/*<Grid container className={classes.group}>*/}
        {/*    <Row label={<DiodeSignalization data={WiFiConnectingSignal}/>}*/}
        {/*         value="Zařízení se připojuje k WiFi nebo se nachází v módu Access Point, aby mohlo být nakonfigurováno."/>*/}
        {/*    <Row label={<DiodeSignalization data={WarmingUpSignal}/>}*/}
        {/*         value="Zařízení bylo právě zapnuto a zahřívají se sensory"/>*/}
        {/*    <Row label={<DiodeSignalization data={WinterDeviceOperationalWifiOnSignal}/>}*/}
        {/*         value="Zařízení je aktivní (a připojené k WiFi), a to v módu vyhřívání"/>*/}
        {/*    <Row label={<DiodeSignalization data={WinterDeviceOperationalWifiOffSignal}/>}*/}
        {/*         value="Zařízení je aktivní (ale odpojené od WiFi), a to v módu vyhřívání"/>*/}
        {/*    <Row label={<DiodeSignalization data={SummerDeviceOperationalWifiOnSignal}/>}*/}
        {/*         value="Zařízení je aktivní (a připojené k WiFi), a to v módu chlazení"/>*/}
        {/*    <Row label={<DiodeSignalization data={SummerDeviceOperationalWifiOffSignal}/>}*/}
        {/*         value="Zařízení je aktivní (ale odpojené od WiFi), a to v módu chlazení"/>*/}
        {/*    <Row label={<DiodeSignalization data={DeviceDisabledWifiOnSignal}/>}*/}
        {/*         value="Zařízení je neaktivní (ale připojené k WiFi) a nedojde k automatickému spuštění"/>*/}
        {/*    <Row label={<DiodeSignalization data={DeviceDisabledWifiOffSignal}/>}*/}
        {/*         value="Zařízení je neaktivní (a odpojené od WiFi) a nedojde k automatickému spuštění"/>*/}
        {/*    <Row label={<DiodeSignalization data={TrialSignal}/>}*/}
        {/*         value="Zařízení je dočasně ovládáno manuálně z webového rozhraní"/>*/}
        {/*    <Row label={<DiodeSignalization data={DisabledDueToConditions}/>}*/}
        {/*         value="Ventilátor je vypnutý kvůli nastaveným podmínkám."/>*/}
        {/*    <Row label={<DiodeSignalization data={CriticalSignal}/>}*/}
        {/*         value="Chybná konfigurace. Vytvořte a uložte novou konfiguraci ve webovém rozhraní."/>*/}
        {/*    <Row label={<DiodeSignalization data={NotValidSensorValues}/>}*/}
        {/*         value="Nelze přečíst hodnoty ze sensorů. Zkontrolujte webové rozhraní a hodnoty čidel."/>*/}
        {/*</Grid>*/}
        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Display</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                Display zobrazuje aktuální hodnoty čidel. Umožňuje také zapnutí tzv. "párty" módu, popř. ventilaci a rekuperaci na chvíli deaktivaci.
                Retrospektivně také ukazuje hodnoty CO2 a vnější teploty.


            </ExpansionPanelDetails>
        </ExpansionPanel>
        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Ovládání dotykového tlačítka</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>

                <Grid container className={classes.group}>
                    <Row label={<><LooksOneIcon/> kliknutí</>}
                         value={<>Při kliknutí se přepne obrazovka na následující.</>}/>
                    <Row label={<><LooksTwo/> 2kliknutí</>}
                         value={<>Při kliknutí se přepne obrazovka na předchozí.</>}/>
                    <Row label={<><TimerIcon/> Podržet 4s</>}
                         value={<>
                             Při potvržení tlačítka se aktivuje akce, která přísluší dané obrazovce.
                         </>}/>
                </Grid>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Změna WiFi připojení</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <p>Pro změnu WiFi sítě (nebo jejího hesla) je potřeba zařízení restartovat, což lze provézt podržením dotykového tlačítka po dobu 10 vteřin.</p>
                <p>Zařízení se po restartu nepodaří připojit k síti a proto přejde do módu Access Point, kdy je možné ho opět konfigurovat.</p>
                <p>
                    <ul>
                        <li>Default SSID: SolarAirCollectorV2</li>
                        <li>Default heslo: 123456789</li>
                    </ul>
                </p>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Sémantika programových kódů</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <div>
                    <p>Do monitoringu se posílá také programový kód, tj. aktuální číslo programu. Každé číslo má konkrétní význam.</p>
                    <p>Toto číslo je nápomocné, pokud se zpětně hodnotí přičina chování systému.</p>
                    <br/>
                    <Grid container className={classes.group}>
                        <Row label={0} value={"Nenastaveno"}/>
                        <Row label={5.1} value={"Nevalidní hodnota z venkovního čidla teploty"}/>
                        <Row label={5.2} value={"Nevalidní hodnota z vnitřního čidla teploty"}/>
                        <Row label={5.3} value={"Nevalidní hodnota z venkovního čidla vlhkosti"}/>
                        <Row label={7.0} value={"Inicializační program"}/>
                        <Row label={8.0} value={"Chybná konfigurace"}/>
                        <Row label={9.0} value={"Program je vypnutý"}/>
                        <Row label={10.0} value={"Manuální program"}/>
                        <Row label={11.0} value={"Chladící program"}/>
                        <Row label={11.1} value={"Chladící program - cílové teploty bylo dosáhnuto"}/>
                        <Row label={11.3} value={"Chladící program - rosný bod překračuje vnitřní teplotu"}/>
                        <Row label={12.0} value={"Vyhřívací program"}/>
                        <Row label={12.1} value={"Vyhřívací program - cílové teploty bylo dosáhnuto"}/>
                        <Row label={12.2} value={"Vyhřívací program - teplota v kolektoru pomalu klesá"}/>
                        <Row label={12.3} value={"Vyhřívací program - rosný bod překračuje vnitřní teplotu"}/>
                    </Grid>
                </div>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        {/*    <div className={classes.center}>*/}
        {/*    <h1>Schéma</h1>*/}
        {/*    <GitHubIcon/> Více o projektu na <a href={"https://gitlab.com/d.bydzovsky/solar-air-collector-esp8266"}>https://gitlab.com/d.bydzovsky/solar-air-collector-esp8266</a><br/>*/}
        {/*</div>*/}

        <InstallationPage/>
    </div>
}


const useStyles = makeStyles((theme: Theme) => ({
    root: {
        margin: 20
    },
    row: {
        marginTop: 20
    },
    row2: {
        marginTop: 20,
        border: "1px dashed black"
    },
    group: {
        marginBottom: 20
    },
    flex: {
        display: "flex",
        alignItems: "center",
        justifyContent: "left",
    },
    header: {
        marginTop: 20,
        fontSize: 15
    },
    center: {
        textAlign: "center"
    }
}));