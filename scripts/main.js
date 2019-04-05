var WEEK_COLOR = {
    'Mon': 'fG',
    'Tue': 'fY',
    'Wed': 'fN',
    'Thu': 'fP',
    'Fri': 'fC',
    'Sat': 'fW',
    'Sun': 'fR',
};


var signatures = {};


function Article (fname) {
    // static info
    this.fname = fname;
    this.index = 0;
    this.like = false;
    this.date = '????/??/??';
    this.time = '??:??';
    this.week = '?';
    this.sign = '';
    this.title = this.fname;
    this.push = [];
    this.re = false;
    this.raw_content = '';
    this.content = [];

    // dynamic info
    this.loaded = false;
    this.read = false;
    this.viewing = false;

    this.parse = function (raw_content) {
        this.raw_content = raw_content;

        var lines = raw_content.split(/\r?\n/g);
        var in_header = true;

        for (var i = 0; i < lines.length; i++) {
            if (in_header) {
                // still in article header
                if (lines[i] == '') {
                    in_header = false;
                }

                var match = /^:(.*?): *(.*)$/.exec(lines[i]);
                if (match == null) {
                    in_header = false;
                    continue;
                }
                // article_info[ match[1] ] = match[2];
                switch (match[1]) {
                    case 'like':
                        this.like = (match[2] === 'true');
                        break;
                    default:
                    this[match[1]] = match[2];
                }
            } else {
                this.content.push(render_line(lines[i]));
            }
        }

        this.content.push('--');
        if (this.sign in signatures) {
            for (i in signatures[this.sign]) {
                this.content.push(render_line(signatures[this.sign][i]));
            }
        }

        this.loaded = true;
    }
}


function render_line (line) {
    if (line == '') {
        return '<br>';
    }

    if (line[0] == '>') {
        line = '[cb;' + line;
    } else if (line[0] == '※') {
        line = '[gb;' + line;
    }


    var regex = /\[(?:([brgynpcwoBRGYNPCWO])([brgynpcwoBRGYNPCWO])(-?))?;/;
    var tokens = []
    while (line) {
        var m = line.match(regex);
        if (m) {
            if (m.index) {
                tokens.push(line.substring(0, m.index));
                line = line.substring(m.index);
            }
            tokens.push(m[0]);
            line = line.replace(regex, '');
        } else {
            tokens.push(line);
            line = '';
        }
    }

    var ret = '';
    var color = false;
    for (var i in tokens) {
        var m = tokens[i].match(regex);
        if (!m) {
            var t = tokens[i];
            t = t.replace(/(^|[^"<])(https?:\/\/[^ ]*)(?![">])/g, '$1<a href="$2" target="_blank">$2</a>');
            t = t.replace(/`([^`]*) +<([^`]*)>`_/g, '<a href="$2" target="_blank">$1</a>');
            ret += t;
            continue;
        }

        if (m[0] == '[;') {
            if (color) {
                ret += '</div>';
            }
            continue;
        }

        if (color) {
            ret += '</div>';
        }
        ret += '<div class="text-block f'+ m[1] +' b'+ m[2] +' '+ (m[3] == '' ? '' : 'stroke') +'">';
        color = true;
    }
    if (color) {
        ret += '</div>';
    }

    return ret;
}


function parse_signatures (raw_content) {
    var lines = raw_content.split(/\r?\n/g);

    var parsing_content = false;
    var signature_name = '';
    for (var i = 0; i < lines.length; i++) {
        if (parsing_content) {
            if (lines[i] == ':end:') {
                parsing_content = false;
            } else {
                signatures[signature_name].push(lines[i]);
            }
        } else {
            var match = /^:start: *(.*)$/.exec(lines[i]);
            if (match != null) {
                signature_name = match[1];
                signatures[signature_name] = [];
                parsing_content = true;
            }
        }
    }
}


$(function () {
    var articles = [];

    for (var i = 0; i < ARTICLE_FILES_LIST.length; i++) {
        var atc = new Article(ARTICLE_FILES_LIST[i]);
        atc.index = i + 1;
        articles.push(atc);

        if (window.location.hash == '#' + atc.fname) {
            atc.viewing = true;
            console.log(atc.fname);
        }
    }


    var render = new Vue({
        el: '#wrapper',
        data: {
            WEEK_COLOR: WEEK_COLOR,
            articles: articles,
            mouse_on_widget: true,
        },
        methods: {
            view_article: function (article) {
                article.viewing = true;
                article.read = true;
                console.log(article.title, article.viewing);
                window.scrollTo(0,0);
            },
            leave_article: function (article) {
                article.viewing = false;
                console.log(article.title, article.viewing);
                window.scrollTo(0,0);
            },
        },
        computed: {
            view_state: function () {
                for (var i in this.articles) {
                    if (this.articles[i].viewing) {
                        return 'article';
                    }
                }
                return 'list';
            }
        },
    });

    $(document).click(function () {
        if (!render.mouse_on_widget) {
            for (var i in render.articles) {
                if (render.articles[i].viewing) {
                    render.articles[i].viewing = false;
                    console.log(render.articles[i].title, render.articles[i].viewing);
                    window.scrollTo(0,0);
                }
            }
        }
    });

    $.ajax({
        url: 'Articles/signatures.txt',
        cache: false,
    }).done(function (msg) {
        parse_signatures(msg);

        download_article(articles);
    });
});


function download_article (articles) {
    for (var i in articles) {
        if (!articles[i].loaded) {
            (function (idx) {
                $.ajax({
                    url: 'Articles/' + articles[idx].fname,
                    cache: false,
                }).done(function (msg) {
                    articles[idx].parse(msg);
                    download_article(articles);
                });
            })(i);
            break;
        }
    }
}
