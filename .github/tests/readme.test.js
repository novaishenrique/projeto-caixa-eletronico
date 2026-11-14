// Confere se os links, as imagens e as âncoras do README apontam para o que existe.
// Localmente: node --test .github/tests/*.test.js
const test = require("node:test");
const assert = require("node:assert");
const fs = require("node:fs");
const path = require("node:path");

const raiz = path.resolve(__dirname, "..", "..");
const ancora = (titulo) =>
  titulo.trim().toLowerCase().replace(/[^\p{L}\p{M}\p{N}\p{Pc} -]/gu, "").replace(/ /g, "-");

test("README: links, imagens e âncoras apontam para o que existe", () => {
  const readme = fs.readFileSync(path.join(raiz, "README.md"), "utf8").replace(/```[\s\S]*?```/g, "");
  const ancoras = new Set([...readme.matchAll(/^#{1,6} (.+)$/gm)].map((m) => ancora(m[1])));
  const links = [
    ...[...readme.matchAll(/\]\(([^)\s]+)\)/g)].map((m) => m[1]),
    ...[...readme.matchAll(/<img [^>]*src="([^"]+)"/g)].map((m) => m[1]),
  ].filter((link) => !/^(https?:|mailto:)/.test(link));
  for (const link of links) {
    const [alvo, fragmento] = decodeURI(link).split("#");
    if (alvo) assert.ok(fs.existsSync(path.join(raiz, alvo)), "link quebrado: " + link);
    else assert.ok(ancoras.has(fragmento), "âncora inexistente: #" + fragmento);
  }
});
