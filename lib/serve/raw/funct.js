window.onload = function() {
    let txtto = null;
    function fetch(val) { return document.getElementById(val); }
    function txtst(msg) {
        if (txtto) { clearTimeout(txtto); }
        txtto = setTimeout(function(){
            fetch('txt-status').innerHTML = '';
            txtto = null;
        }, 3000);
        fetch('txt-status').innerHTML = msg;
    }
    function batch(callback) {
        const col = ['bbb', 'ggg', 'rrr'];
        let idx = col.length;
        while (idx--) { callback(col[idx]); }
    }
    function paint() {
        function hexler(num) {
            const res = Number(num).toString(16);
            return String((num < 16) ? '0' + res : res).toUpperCase();
        }
        let result = '#';
        batch(function(channel) {
            const stain = hexler(fetch('slide-' + channel).value);
            fetch('color-' + channel).innerHTML = stain;
            result += stain;
        });
        fetch('color-rgb').style.backgroundColor = result;
        return result;
    }
    function check(data, callback, arg) {
        if (data !== undefined && data !== null) { callback(data, arg); }
    }
    function status(response) {
        function tset(val, name) { fetch('txt-' + name).innerHTML = val; }
        check(response, function(data) {
            check(data.channel, tset, 'channel');
            check(data.dialups, tset, 'dialups');
            check(data.hangups, tset, 'hangups');
            check(data.launched, tset, 'launched');
            check(data.requests, tset, 'requests');
            check(data.signal, tset, 'signal');
            check(data.uptime, tset, 'uptime');
            check(data.color, function(col) {
                document.body.style.backgroundColor = col;
            });
            txtst('ok');
        });
    }
    function toggle(response) {
        check(response, function(data) {
            check(data.action, function(act) {
                if (act === "fade") { status(data); }
            });
            check(data.status, txtst);
        });
    }
    function query(endpoint, callback) {
        txtst('query ' + endpoint + ' &hellip;');
        const xhr = new XMLHttpRequest();
        xhr.open('GET', '/' + endpoint, true);
        xhr.onreadystatechange = function() {
            if (
                xhr.readyState === 4 &&
                xhr.status >= 200 && xhr.status < 400 &&
                xhr.responseText
            ) {
                callback(JSON.parse(xhr.responseText));
            } else { txtst('query ' + endpoint + ' error'); }
        };
        xhr.send();
    }
    function light(btn) {
        query(
            'light/' + btn.target.id + '/' + paint().replace("#", "%23"),
            toggle
        );
    }
    function power(btn) {
        query(
            'power/' + btn.target.id.replace('-', '/'),
            toggle
        );
    }
    function setup() {
        txtst('setup');
        batch(function(channel) {
            const slide = fetch('slide-' + channel);
            slide.value = Math.floor(Math.random() * 0xff);
            slide.addEventListener('input', paint);
        });
        paint();
        const inp = document.getElementsByTagName('input');
        let idx = inp.length;
        while (idx--) { if (inp[idx].type === 'button') {
            inp[idx].addEventListener('click', (
                inp[idx].id === 'flash' || inp[idx].id === 'fade'
            ) ? light : power);
        }}
        setInterval(function() { query('stats', status); }, __update__);
        status(__status__);
    }
    setup();
};
