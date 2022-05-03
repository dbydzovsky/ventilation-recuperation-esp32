import React, {useEffect} from 'react'
import {makeStyles} from "@material-ui/styles";
import {Theme} from "@material-ui/core/styles";
import {Container, useMediaQuery} from "@material-ui/core";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {getDebugMessagesAsync} from "../actions/debug";

export function DebugMessagesLoader() {
    const dispatch = useDispatch()
    useEffect(() => {
        let isSubscribed = true;
        let schedule = () => {
            dispatch(getDebugMessagesAsync({
                onDone: () => {
                    setTimeout(() => {
                        if (isSubscribed) schedule();
                    }, 2000);
                }
            }))
        }
        schedule();
        return () => {
            isSubscribed = false
        }
    }, []);
    return <div></div>
}

export function DebugMessagesPage() {
    const classes = useStyles();
    const debugState = useSelector((state: RootState) => state.debugState);
    return <div>
        <DebugMessagesLoader/>
        <div className={classes.outerMessages}>
            <Container>
                <div className={classes.messages}>
                    {debugState.messages.map((message) => {
                        let clazz= classes.message;
                        if (message.severity == "WARN") {
                            clazz += " " +classes.warn;
                        }
                        if (message.severity == "ERR") {
                            clazz += " " +classes.err;
                        }
                        return <span className={clazz}><code >{"> "}{ message.time }{message.time ? ": " : "" }{message.text}</code></span>
                    })}
                </div>
            </Container>
        </div>
    </div>
}


const useStyles = makeStyles((theme: Theme) => ({
    messages: {
        display: "grid",
        [theme.breakpoints.down("sm")]: {
            fontSize: "0.8em"
        }
    },
    outerMessages: {
        color: "#ddd",
        background: "#333",
        margin: "10px 0px 10px 0px ",
        width: "100%",
    },
    message: {

    },
    warn: {
        color: "orange"
    },
    err: {
        color: "red"
    },
    root: {
        margin: "auto"
    },
    row: {
        marginTop: 20
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