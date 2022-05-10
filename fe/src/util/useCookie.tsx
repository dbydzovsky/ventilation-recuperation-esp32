import React, { useState } from "react";

const getItem = (key: string) =>
    document.cookie.split("; ").reduce((total, currentCookie) => {
        const item = currentCookie.split("=");
        const storedKey = item[0];
        const storedValue = item[1];

        return key === storedKey ? decodeURIComponent(storedValue) : total;
    }, "");

const setItem = (key: string, value: number, numberOfDays: number) => {
    const now = new Date();

    // set the time to be now + numberOfDays
    now.setTime(now.getTime() + numberOfDays * 60 * 60 * 24 * 1000);

    document.cookie = `${key}=${value}; expires=${now.toUTCString()}; path=/`;
};
const useCookie = (key: string, defaultValue: any) => {
    const getCookie = () => getItem(key) || defaultValue;
    const [cookie, setCookie] = useState(getCookie());

    const updateCookie = (value: any) => {
        setCookie(value);
        const numberOfDays = 365;
        setItem(key, value, numberOfDays);
    };

    return [cookie, updateCookie];
};

export default useCookie;
