#pragma once

//enum { differTemplateSignature };
#define TMPL_COND_DECL(cond)  typename = typename std::enable_if<(cond)>::type
#define TMPL_COND_DEFI typename
#define TMPL_SIG_DECL typename = void
#define TMPL_SIG_DEFI typename
