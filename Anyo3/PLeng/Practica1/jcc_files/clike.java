/* Generated By:JavaCC: Do not edit this line. clike.java */
package traductor;

public class clike implements clikeConstants {
    public static void main(String[] args) {
        clike parser = null;

        try {
                if(args.length == 0) {
                                parser = new clike(System.in);
                        }
                        else {
                    parser = new clike(new java.io.FileInputStream(args[0]));
                        }
                        //Programa es el símbolo inicial de la gramática
                        parser.Programa();
                        System.out.println("***** An\u00e1lisis terminado con \u00e9xito *****");
                }
                catch (java.io.FileNotFoundException e) {
                        System.err.println ("Fichero " + args[0] + " no encontrado.");
                }
                catch (TokenMgrError e) {
                System.err.println("ERROR L\u00c9XICO: " + e.getMessage());
        }
                catch (ParseException e) {
            System.err.println("PARSER_ERROR: " + e.getMessage());
        }
    }

        public static void printToken(String s, Token t) {
                System.out.printf("(%d,%d): %s \"%s\"\n", t.beginLine, t.beginColumn, s, t.image);
        }

//------------ Símbolo inicial de la gramática. Para análisis léxico no hace falta más
  static final public void Programa() throws ParseException {
        Token t;
    label_1:
    while (true) {
      switch ((jj_ntk==-1)?jj_ntk():jj_ntk) {
      case tBOOL:
        jj_consume_token(tBOOL);
        break;
      case tCHAR:
        jj_consume_token(tCHAR);
        break;
      case tINT:
        jj_consume_token(tINT);
        break;
      case tVOID:
        jj_consume_token(tVOID);
        break;
      case tIF:
        jj_consume_token(tIF);
        break;
      case tELSE:
        jj_consume_token(tELSE);
        break;
      case tWHILE:
        jj_consume_token(tWHILE);
        break;
      case tRETURN:
        jj_consume_token(tRETURN);
        break;
      case tPAREN_INI:
        jj_consume_token(tPAREN_INI);
        break;
      case tPAREN_FIN:
        jj_consume_token(tPAREN_FIN);
        break;
      case tLLAVE_INI:
        jj_consume_token(tLLAVE_INI);
        break;
      case tLLAVE_FIN:
        jj_consume_token(tLLAVE_FIN);
        break;
      case tCOMA:
        jj_consume_token(tCOMA);
        break;
      case tPUNTO_COMA:
        jj_consume_token(tPUNTO_COMA);
        break;
      case tASIGNAR:
        jj_consume_token(tASIGNAR);
        break;
      case tSUMA:
        jj_consume_token(tSUMA);
        break;
      case tRESTA:
        jj_consume_token(tRESTA);
        break;
      case tMULTI:
        jj_consume_token(tMULTI);
        break;
      case tDIV:
        jj_consume_token(tDIV);
        break;
      case tMODULO:
        jj_consume_token(tMODULO);
        break;
      case tMAYOR:
        jj_consume_token(tMAYOR);
        break;
      case tMAYOR_IGUAL:
        jj_consume_token(tMAYOR_IGUAL);
        break;
      case tMENOR:
        jj_consume_token(tMENOR);
        break;
      case tMENOR_IGUAL:
        jj_consume_token(tMENOR_IGUAL);
        break;
      case tIGUAL:
        jj_consume_token(tIGUAL);
        break;
      case tDISTINTO:
        jj_consume_token(tDISTINTO);
        break;
      case tAND:
        jj_consume_token(tAND);
        break;
      case tOR:
        jj_consume_token(tOR);
        break;
      case tNOT:
        jj_consume_token(tNOT);
        break;
      case tCORCH_INI:
        jj_consume_token(tCORCH_INI);
        break;
      case tCORCH_FIN:
        jj_consume_token(tCORCH_FIN);
        break;
      case tIDENTIFICADOR:
        jj_consume_token(tIDENTIFICADOR);
        break;
      case tCONST_NUM:
        jj_consume_token(tCONST_NUM);
        break;
      case tCONST_CHAR:
        jj_consume_token(tCONST_CHAR);
        break;
      case tCONST_STRING:
        jj_consume_token(tCONST_STRING);
        break;
      case tTRUE:
        jj_consume_token(tTRUE);
        break;
      case tFALSE:
        jj_consume_token(tFALSE);
        break;
      case tAMPERSAND:
        jj_consume_token(tAMPERSAND);
        break;
      default:
        jj_la1[0] = jj_gen;
        jj_consume_token(-1);
        throw new ParseException();
      }
      switch ((jj_ntk==-1)?jj_ntk():jj_ntk) {
      case tBOOL:
      case tCHAR:
      case tINT:
      case tVOID:
      case tIF:
      case tELSE:
      case tWHILE:
      case tRETURN:
      case tPAREN_INI:
      case tPAREN_FIN:
      case tLLAVE_INI:
      case tLLAVE_FIN:
      case tCORCH_INI:
      case tCORCH_FIN:
      case tCOMA:
      case tPUNTO_COMA:
      case tASIGNAR:
      case tSUMA:
      case tRESTA:
      case tMULTI:
      case tDIV:
      case tMODULO:
      case tMAYOR:
      case tMENOR:
      case tMAYOR_IGUAL:
      case tMENOR_IGUAL:
      case tIGUAL:
      case tDISTINTO:
      case tAND:
      case tOR:
      case tNOT:
      case tAMPERSAND:
      case tIDENTIFICADOR:
      case tCONST_CHAR:
      case tCONST_STRING:
      case tCONST_NUM:
      case tTRUE:
      case tFALSE:
        ;
        break;
      default:
        jj_la1[1] = jj_gen;
        break label_1;
      }
    }
    jj_consume_token(0);

  }

  static private boolean jj_initialized_once = false;
  /** Generated Token Manager. */
  static public clikeTokenManager token_source;
  static SimpleCharStream jj_input_stream;
  /** Current token. */
  static public Token token;
  /** Next token. */
  static public Token jj_nt;
  static private int jj_ntk;
  static private int jj_gen;
  static final private int[] jj_la1 = new int[2];
  static private int[] jj_la1_0;
  static private int[] jj_la1_1;
  static {
      jj_la1_init_0();
      jj_la1_init_1();
   }
   private static void jj_la1_init_0() {
      jj_la1_0 = new int[] {0xfffffff8,0xfffffff8,};
   }
   private static void jj_la1_init_1() {
      jj_la1_1 = new int[] {0x1ff,0x1ff,};
   }

  /** Constructor with InputStream. */
  public clike(java.io.InputStream stream) {
     this(stream, null);
  }
  /** Constructor with InputStream and supplied encoding */
  public clike(java.io.InputStream stream, String encoding) {
    if (jj_initialized_once) {
      System.out.println("ERROR: Second call to constructor of static parser.  ");
      System.out.println("       You must either use ReInit() or set the JavaCC option STATIC to false");
      System.out.println("       during parser generation.");
      throw new Error();
    }
    jj_initialized_once = true;
    try { jj_input_stream = new SimpleCharStream(stream, encoding, 1, 1); } catch(java.io.UnsupportedEncodingException e) { throw new RuntimeException(e); }
    token_source = new clikeTokenManager(jj_input_stream);
    token = new Token();
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 2; i++) jj_la1[i] = -1;
  }

  /** Reinitialise. */
  static public void ReInit(java.io.InputStream stream) {
     ReInit(stream, null);
  }
  /** Reinitialise. */
  static public void ReInit(java.io.InputStream stream, String encoding) {
    try { jj_input_stream.ReInit(stream, encoding, 1, 1); } catch(java.io.UnsupportedEncodingException e) { throw new RuntimeException(e); }
    token_source.ReInit(jj_input_stream);
    token = new Token();
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 2; i++) jj_la1[i] = -1;
  }

  /** Constructor. */
  public clike(java.io.Reader stream) {
    if (jj_initialized_once) {
      System.out.println("ERROR: Second call to constructor of static parser. ");
      System.out.println("       You must either use ReInit() or set the JavaCC option STATIC to false");
      System.out.println("       during parser generation.");
      throw new Error();
    }
    jj_initialized_once = true;
    jj_input_stream = new SimpleCharStream(stream, 1, 1);
    token_source = new clikeTokenManager(jj_input_stream);
    token = new Token();
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 2; i++) jj_la1[i] = -1;
  }

  /** Reinitialise. */
  static public void ReInit(java.io.Reader stream) {
    jj_input_stream.ReInit(stream, 1, 1);
    token_source.ReInit(jj_input_stream);
    token = new Token();
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 2; i++) jj_la1[i] = -1;
  }

  /** Constructor with generated Token Manager. */
  public clike(clikeTokenManager tm) {
    if (jj_initialized_once) {
      System.out.println("ERROR: Second call to constructor of static parser. ");
      System.out.println("       You must either use ReInit() or set the JavaCC option STATIC to false");
      System.out.println("       during parser generation.");
      throw new Error();
    }
    jj_initialized_once = true;
    token_source = tm;
    token = new Token();
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 2; i++) jj_la1[i] = -1;
  }

  /** Reinitialise. */
  public void ReInit(clikeTokenManager tm) {
    token_source = tm;
    token = new Token();
    jj_ntk = -1;
    jj_gen = 0;
    for (int i = 0; i < 2; i++) jj_la1[i] = -1;
  }

  static private Token jj_consume_token(int kind) throws ParseException {
    Token oldToken;
    if ((oldToken = token).next != null) token = token.next;
    else token = token.next = token_source.getNextToken();
    jj_ntk = -1;
    if (token.kind == kind) {
      jj_gen++;
      return token;
    }
    token = oldToken;
    jj_kind = kind;
    throw generateParseException();
  }


/** Get the next Token. */
  static final public Token getNextToken() {
    if (token.next != null) token = token.next;
    else token = token.next = token_source.getNextToken();
    jj_ntk = -1;
    jj_gen++;
    return token;
  }

/** Get the specific Token. */
  static final public Token getToken(int index) {
    Token t = token;
    for (int i = 0; i < index; i++) {
      if (t.next != null) t = t.next;
      else t = t.next = token_source.getNextToken();
    }
    return t;
  }

  static private int jj_ntk() {
    if ((jj_nt=token.next) == null)
      return (jj_ntk = (token.next=token_source.getNextToken()).kind);
    else
      return (jj_ntk = jj_nt.kind);
  }

  static private java.util.List jj_expentries = new java.util.ArrayList();
  static private int[] jj_expentry;
  static private int jj_kind = -1;

  /** Generate ParseException. */
  static public ParseException generateParseException() {
    jj_expentries.clear();
    boolean[] la1tokens = new boolean[45];
    if (jj_kind >= 0) {
      la1tokens[jj_kind] = true;
      jj_kind = -1;
    }
    for (int i = 0; i < 2; i++) {
      if (jj_la1[i] == jj_gen) {
        for (int j = 0; j < 32; j++) {
          if ((jj_la1_0[i] & (1<<j)) != 0) {
            la1tokens[j] = true;
          }
          if ((jj_la1_1[i] & (1<<j)) != 0) {
            la1tokens[32+j] = true;
          }
        }
      }
    }
    for (int i = 0; i < 45; i++) {
      if (la1tokens[i]) {
        jj_expentry = new int[1];
        jj_expentry[0] = i;
        jj_expentries.add(jj_expentry);
      }
    }
    int[][] exptokseq = new int[jj_expentries.size()][];
    for (int i = 0; i < jj_expentries.size(); i++) {
      exptokseq[i] = (int[])jj_expentries.get(i);
    }
    return new ParseException(token, exptokseq, tokenImage);
  }

  /** Enable tracing. */
  static final public void enable_tracing() {
  }

  /** Disable tracing. */
  static final public void disable_tracing() {
  }

}