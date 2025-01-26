class WordCount extends HTMLElement {
  static observedAttributes = ["size"];
  constructor() {
    super();
  }
  connectedCallback() {
    console.log("自定义元素添加至页面。");
  }

  disconnectedCallback() {
    console.log("自定义元素从页面中移除。");
  }

  adoptedCallback() {
    console.log("自定义元素移动至新页面。");
  }

  attributeChangedCallback(name, oldValue, newValue) {
    console.log(`属性 ${name} 已变更。从${oldValue}变为${newValue}`);
  }
}

customElements.define("word-count", WordCount);
const pEle = document.querySelector("#para");
const shadow = pEle.attachShadow({ mode: "open" });
const sheet = new CSSStyleSheet();
sheet.replaceSync("span {color: red;}");
shadow.adoptedStyleSheets = [sheet];
const span = document.createElement("span");
span.appendChild(document.createTextNode("这是一个span元素"));
shadow.appendChild(document.createTextNode("这是一个自定义元素"));
shadow.appendChild(span);
const spans = shadow.querySelectorAll("span");
for (const span of spans) {
  span.textContent = span.textContent.toLocaleLowerCase();
}
const wc = document.getElementById("wc");
const host = document.getElementById("host");
const other_shadow = host.attachShadow({ mode: "open" });
other_shadow.appendChild(wc.content);

class MyParagraph extends HTMLElement {
  static observedAttributes = ["margin"];
  constructor() {
    super();
    let template = document.getElementById("my-paragraph");
    let templateContent = template.content;
    this._shadowRoot = this.attachShadow({ mode: "open" });
    this._shadowRoot.appendChild(templateContent.cloneNode(true));
    
  }
  connectedCallback() {
    console.log("自定义元素添加至页面。");
  }
  attributeChangedCallback(name, oldValue, newValue) {
    console.log(`属性 ${name} 已变更。从${oldValue}变为${newValue}`);
    if (name == "margin") {
      this._shadowRoot.querySelector("p").style.margin = newValue+'px';
    }
  }
}
customElements.define("my-paragraph", MyParagraph);
