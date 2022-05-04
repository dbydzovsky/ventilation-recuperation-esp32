import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {Images} from "../images/Images";

export interface Props {

}

export function DisplayHelpPart(props: Props) {
    const classes = useStyles()
    return <div className={classes.root}>
        <ScreenShowcase img={Images.mainscreen1}/>
        <ScreenShowcase img={Images.mainScreen2}/>
        <ScreenShowcase img={Images.alarmScreen}/>
        <ScreenShowcase img={Images.historyCo2Screen}/>
        <ScreenShowcase img={Images.tempHistory}/>
        <ScreenShowcase img={Images.disableScreen}/>
        <ScreenShowcase img={Images.party}/>
        <ScreenShowcase img={Images.manualScreen}/>
        <ScreenShowcase img={Images.inhouseScreen}/>
        <ScreenShowcase img={Images.outscreen}/>
        <ScreenShowcase img={Images.debug}/>
    </div>
}

function ScreenShowcase(props: {img: {src: string}}) {
    return <div>
        {props.img.src}
    </div>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        }
    }
})