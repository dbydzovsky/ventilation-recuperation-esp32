import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {Images} from "../images/Images";
import Grid from "@material-ui/core/Grid";

export interface Props {

}

export function DisplayHelpPart(props: Props) {
    const classes = useStyles()
    return <Grid container className={classes.root}>
        <ScreenShowcase img={Images.mainscreen1} txt={"Hlavní obrazovka zobrazuje aktuální čas, stav připojení k wifi, teplotu v kolektoru a CO2 hodnotu."}/>
        <ScreenShowcase img={Images.mainScreen2} txt={"Aktuální čas se střídá se zobrazováním aktuální předpovědi pocitové teploty na následující den."}/>
        <ScreenShowcase img={Images.alarmScreen} txt={"V případě neočekávaných problémů problikává hlavní obrazovka podobným textem."}/>
        <ScreenShowcase img={Images.historyCo2Screen} txt={"Historie naměřených CO2 hodnot za poslední 2 hodiny. Čísla v grafu uvádějí krajní hodnoty."}/>
        <ScreenShowcase img={Images.tempHistory} txt={"Historie naměřených hodnot teploty v kolektoru za poslední 2 hodiny. Čísla v grafu uvádějí krajní hodnoty."}/>
        <ScreenShowcase img={Images.disableScreen} txt={"Obrazovka s akcí, kterou je možné dočasně vypnout zařízení."}/>
        <ScreenShowcase img={Images.party} txt={"Obrazovka s akcí, kterou je možné zapnout párty mód."}/>
        <ScreenShowcase img={Images.manualScreen} txt={"Manuální kontrola zařízení. Ukazuje aktuální teplotu v kolektoru, aktuální CO2 a čas, který zbývá do konce programu."}/>
        <ScreenShowcase img={Images.inhouseScreen} txt={"Informace o vnitřních čidlech. Ukazuje aktuální rosný bod, teplotu, relativní vlhkost a CO2."}/>
        <ScreenShowcase img={Images.outscreen} txt={"Informace o venkovních čidlech. Ukazuje aktuální rosný bod, teplotu a relativní vlhkost v kolektoru a předpovídanou pocitovou teplotu na následující den."}/>
        <ScreenShowcase img={Images.debug} txt={"Ladící obrazovka. Zobrazuje aktuální verzi programu, množství zbývající paměti, celkový čas běhu, číslo programu a rok výroby."}/>
    </Grid>
}
interface ShowCaseProps {
    txt: string
    img: {src: string}
}
function ScreenShowcase(props: ShowCaseProps) {
    const classes = useStyles();
    return <Grid item xs={12} sm={6} md={4} className={classes.imgwrap}>
        <img src={props.img.src} className={classes.img}/><br/>
        {props.txt}

    </Grid>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        },
        imgwrap: {
            textAlign: "center",
            padding: theme.spacing(2)
        },
        img: {
            textAlign: "center"
        }
    }
})