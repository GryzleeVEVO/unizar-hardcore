/* Generated By:JavaCC: Do not edit this line. clikeTokenManager.java */
package traductor;

/** Token Manager. */
public class clikeTokenManager implements clikeConstants
{

  /** Debug output. */
  public static  java.io.PrintStream debugStream = System.out;
  /** Set debug output. */
  public static  void setDebugStream(java.io.PrintStream ds) { debugStream = ds; }
private static final int jjStopStringLiteralDfa_0(int pos, long active0)
{
   switch (pos)
   {
      case 0:
         if ((active0 & 0x200000L) != 0L)
            return 2;
         if ((active0 & 0x7f8L) != 0L)
            return 1;
         return -1;
      case 1:
         if ((active0 & 0x80L) != 0L)
            return 1;
         if ((active0 & 0x778L) != 0L)
         {
            jjmatchedKind = 31;
            jjmatchedPos = 1;
            return 1;
         }
         return -1;
      case 2:
         if ((active0 & 0x20L) != 0L)
            return 1;
         if ((active0 & 0x758L) != 0L)
         {
            jjmatchedKind = 31;
            jjmatchedPos = 2;
            return 1;
         }
         return -1;
      case 3:
         if ((active0 & 0x158L) != 0L)
            return 1;
         if ((active0 & 0x600L) != 0L)
         {
            jjmatchedKind = 31;
            jjmatchedPos = 3;
            return 1;
         }
         return -1;
      case 4:
         if ((active0 & 0x400L) != 0L)
         {
            jjmatchedKind = 31;
            jjmatchedPos = 4;
            return 1;
         }
         if ((active0 & 0x200L) != 0L)
            return 1;
         return -1;
      default :
         return -1;
   }
}
private static final int jjStartNfa_0(int pos, long active0)
{
   return jjMoveNfa_0(jjStopStringLiteralDfa_0(pos, active0), pos + 1);
}
static private int jjStopAtPos(int pos, int kind)
{
   jjmatchedKind = kind;
   jjmatchedPos = pos;
   return pos + 1;
}
static private int jjStartNfaWithStates_0(int pos, int kind, int state)
{
   jjmatchedKind = kind;
   jjmatchedPos = pos;
   try { curChar = input_stream.readChar(); }
   catch(java.io.IOException e) { return pos + 1; }
   return jjMoveNfa_0(state, pos + 1);
}
static private int jjMoveStringLiteralDfa0_0()
{
   switch(curChar)
   {
      case 37:
         return jjStopAtPos(0, 22);
      case 40:
         return jjStopAtPos(0, 11);
      case 41:
         return jjStopAtPos(0, 12);
      case 42:
         return jjStopAtPos(0, 20);
      case 43:
         return jjStopAtPos(0, 18);
      case 44:
         return jjStopAtPos(0, 15);
      case 45:
         return jjStopAtPos(0, 19);
      case 47:
         return jjStartNfaWithStates_0(0, 21, 2);
      case 59:
         return jjStopAtPos(0, 16);
      case 60:
         jjmatchedKind = 24;
         return jjMoveStringLiteralDfa1_0(0x14000000L);
      case 61:
         jjmatchedKind = 17;
         return jjMoveStringLiteralDfa1_0(0x8000000L);
      case 62:
         jjmatchedKind = 23;
         return jjMoveStringLiteralDfa1_0(0x2000000L);
      case 91:
         return jjStopAtPos(0, 29);
      case 93:
         return jjStopAtPos(0, 30);
      case 98:
         return jjMoveStringLiteralDfa1_0(0x8L);
      case 99:
         return jjMoveStringLiteralDfa1_0(0x10L);
      case 101:
         return jjMoveStringLiteralDfa1_0(0x100L);
      case 105:
         return jjMoveStringLiteralDfa1_0(0xa0L);
      case 114:
         return jjMoveStringLiteralDfa1_0(0x400L);
      case 118:
         return jjMoveStringLiteralDfa1_0(0x40L);
      case 119:
         return jjMoveStringLiteralDfa1_0(0x200L);
      case 123:
         return jjStopAtPos(0, 13);
      case 125:
         return jjStopAtPos(0, 14);
      default :
         return jjMoveNfa_0(0, 0);
   }
}
static private int jjMoveStringLiteralDfa1_0(long active0)
{
   try { curChar = input_stream.readChar(); }
   catch(java.io.IOException e) {
      jjStopStringLiteralDfa_0(0, active0);
      return 1;
   }
   switch(curChar)
   {
      case 61:
         if ((active0 & 0x2000000L) != 0L)
            return jjStopAtPos(1, 25);
         else if ((active0 & 0x4000000L) != 0L)
            return jjStopAtPos(1, 26);
         else if ((active0 & 0x8000000L) != 0L)
            return jjStopAtPos(1, 27);
         break;
      case 62:
         if ((active0 & 0x10000000L) != 0L)
            return jjStopAtPos(1, 28);
         break;
      case 101:
         return jjMoveStringLiteralDfa2_0(active0, 0x400L);
      case 102:
         if ((active0 & 0x80L) != 0L)
            return jjStartNfaWithStates_0(1, 7, 1);
         break;
      case 104:
         return jjMoveStringLiteralDfa2_0(active0, 0x210L);
      case 108:
         return jjMoveStringLiteralDfa2_0(active0, 0x100L);
      case 110:
         return jjMoveStringLiteralDfa2_0(active0, 0x20L);
      case 111:
         return jjMoveStringLiteralDfa2_0(active0, 0x48L);
      default :
         break;
   }
   return jjStartNfa_0(0, active0);
}
static private int jjMoveStringLiteralDfa2_0(long old0, long active0)
{
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(0, old0); 
   try { curChar = input_stream.readChar(); }
   catch(java.io.IOException e) {
      jjStopStringLiteralDfa_0(1, active0);
      return 2;
   }
   switch(curChar)
   {
      case 97:
         return jjMoveStringLiteralDfa3_0(active0, 0x10L);
      case 105:
         return jjMoveStringLiteralDfa3_0(active0, 0x240L);
      case 111:
         return jjMoveStringLiteralDfa3_0(active0, 0x8L);
      case 115:
         return jjMoveStringLiteralDfa3_0(active0, 0x100L);
      case 116:
         if ((active0 & 0x20L) != 0L)
            return jjStartNfaWithStates_0(2, 5, 1);
         return jjMoveStringLiteralDfa3_0(active0, 0x400L);
      default :
         break;
   }
   return jjStartNfa_0(1, active0);
}
static private int jjMoveStringLiteralDfa3_0(long old0, long active0)
{
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(1, old0); 
   try { curChar = input_stream.readChar(); }
   catch(java.io.IOException e) {
      jjStopStringLiteralDfa_0(2, active0);
      return 3;
   }
   switch(curChar)
   {
      case 100:
         if ((active0 & 0x40L) != 0L)
            return jjStartNfaWithStates_0(3, 6, 1);
         break;
      case 101:
         if ((active0 & 0x100L) != 0L)
            return jjStartNfaWithStates_0(3, 8, 1);
         break;
      case 108:
         if ((active0 & 0x8L) != 0L)
            return jjStartNfaWithStates_0(3, 3, 1);
         return jjMoveStringLiteralDfa4_0(active0, 0x200L);
      case 114:
         if ((active0 & 0x10L) != 0L)
            return jjStartNfaWithStates_0(3, 4, 1);
         break;
      case 117:
         return jjMoveStringLiteralDfa4_0(active0, 0x400L);
      default :
         break;
   }
   return jjStartNfa_0(2, active0);
}
static private int jjMoveStringLiteralDfa4_0(long old0, long active0)
{
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(2, old0); 
   try { curChar = input_stream.readChar(); }
   catch(java.io.IOException e) {
      jjStopStringLiteralDfa_0(3, active0);
      return 4;
   }
   switch(curChar)
   {
      case 101:
         if ((active0 & 0x200L) != 0L)
            return jjStartNfaWithStates_0(4, 9, 1);
         break;
      case 114:
         return jjMoveStringLiteralDfa5_0(active0, 0x400L);
      default :
         break;
   }
   return jjStartNfa_0(3, active0);
}
static private int jjMoveStringLiteralDfa5_0(long old0, long active0)
{
   if (((active0 &= old0)) == 0L)
      return jjStartNfa_0(3, old0); 
   try { curChar = input_stream.readChar(); }
   catch(java.io.IOException e) {
      jjStopStringLiteralDfa_0(4, active0);
      return 5;
   }
   switch(curChar)
   {
      case 110:
         if ((active0 & 0x400L) != 0L)
            return jjStartNfaWithStates_0(5, 10, 1);
         break;
      default :
         break;
   }
   return jjStartNfa_0(4, active0);
}
static final long[] jjbitVec0 = {
   0x0L, 0x0L, 0xffffffffffffffffL, 0xffffffffffffffffL
};
static private int jjMoveNfa_0(int startState, int curPos)
{
   //int[] nextStates; // not used
   int startsAt = 0;
   jjnewStateCnt = 5;
   int i = 1;
   jjstateSet[0] = startState;
   //int j; // not used
   int kind = 0x7fffffff;
   for (;;)
   {
      if (++jjround == 0x7fffffff)
         ReInitRounds();
      if (curChar < 64)
      {
         long l = 1L << curChar;
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if (curChar == 47)
                     jjstateSet[jjnewStateCnt++] = 2;
                  break;
               case 1:
                  if ((0x3ff000000000000L & l) == 0L)
                     break;
                  if (kind > 31)
                     kind = 31;
                  jjstateSet[jjnewStateCnt++] = 1;
                  break;
               case 2:
                  if (curChar == 47)
                     jjCheckNAdd(3);
                  break;
               case 3:
                  if ((0xfffffffffffffbffL & l) == 0L)
                     break;
                  if (kind > 35)
                     kind = 35;
                  jjCheckNAdd(3);
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else if (curChar < 128)
      {
         long l = 1L << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 0:
                  if ((0x7fffffe87fffffeL & l) != 0L)
                     jjCheckNAdd(1);
                  break;
               case 1:
                  if ((0x7fffffe87fffffeL & l) == 0L)
                     break;
                  if (kind > 31)
                     kind = 31;
                  jjCheckNAdd(1);
                  break;
               case 3:
                  if (kind > 35)
                     kind = 35;
                  jjstateSet[jjnewStateCnt++] = 3;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      else
      {
         int i2 = (curChar & 0xff) >> 6;
         long l2 = 1L << (curChar & 077);
         do
         {
            switch(jjstateSet[--i])
            {
               case 3:
                  if ((jjbitVec0[i2] & l2) == 0L)
                     break;
                  if (kind > 35)
                     kind = 35;
                  jjstateSet[jjnewStateCnt++] = 3;
                  break;
               default : break;
            }
         } while(i != startsAt);
      }
      if (kind != 0x7fffffff)
      {
         jjmatchedKind = kind;
         jjmatchedPos = curPos;
         kind = 0x7fffffff;
      }
      ++curPos;
      if ((i = jjnewStateCnt) == (startsAt = 5 - (jjnewStateCnt = startsAt)))
         return curPos;
      try { curChar = input_stream.readChar(); }
      catch(java.io.IOException e) { return curPos; }
   }
}
static final int[] jjnextStates = {
};

/** Token literal values. */
public static final String[] jjstrLiteralImages = {
"", null, null, "\142\157\157\154", "\143\150\141\162", "\151\156\164", 
"\166\157\151\144", "\151\146", "\145\154\163\145", "\167\150\151\154\145", 
"\162\145\164\165\162\156", "\50", "\51", "\173", "\175", "\54", "\73", "\75", "\53", "\55", "\52", "\57", 
"\45", "\76", "\74", "\76\75", "\74\75", "\75\75", "\74\76", "\133", "\135", null, 
null, null, null, null, };

/** Lexer state names. */
public static final String[] lexStateNames = {
   "DEFAULT", 
};
static final long[] jjtoToken = {
   0xfffffff9L, 
};
static final long[] jjtoSkip = {
   0xf00000000L, 
};
static protected SimpleCharStream input_stream;
static private final int[] jjrounds = new int[5];
static private final int[] jjstateSet = new int[10];
static StringBuffer image;
static int jjimageLen;
static int lengthOfMatch;
static protected char curChar;
/** Constructor. */
public clikeTokenManager(SimpleCharStream stream){
   if (input_stream != null)
      throw new TokenMgrError("ERROR: Second call to constructor of static lexer. You must use ReInit() to initialize the static variables.", TokenMgrError.STATIC_LEXER_ERROR);
   input_stream = stream;
}

/** Constructor. */
public clikeTokenManager(SimpleCharStream stream, int lexState){
   this(stream);
   SwitchTo(lexState);
}

/** Reinitialise parser. */
static public void ReInit(SimpleCharStream stream)
{
   jjmatchedPos = jjnewStateCnt = 0;
   curLexState = defaultLexState;
   input_stream = stream;
   ReInitRounds();
}
static private void ReInitRounds()
{
   int i;
   jjround = 0x80000001;
   for (i = 5; i-- > 0;)
      jjrounds[i] = 0x80000000;
}

/** Reinitialise parser. */
static public void ReInit(SimpleCharStream stream, int lexState)
{
   ReInit(stream);
   SwitchTo(lexState);
}

/** Switch to specified lex state. */
static public void SwitchTo(int lexState)
{
   if (lexState >= 1 || lexState < 0)
      throw new TokenMgrError("Error: Ignoring invalid lexical state : " + lexState + ". State unchanged.", TokenMgrError.INVALID_LEXICAL_STATE);
   else
      curLexState = lexState;
}

static protected Token jjFillToken()
{
   final Token t;
   final String tokenImage;
   final int beginLine;
   final int endLine;
   final int beginColumn;
   final int endColumn;
   String im = jjstrLiteralImages[jjmatchedKind];
   tokenImage = (im == null) ? input_stream.GetImage() : im;
   beginLine = input_stream.getBeginLine();
   beginColumn = input_stream.getBeginColumn();
   endLine = input_stream.getEndLine();
   endColumn = input_stream.getEndColumn();
   t = Token.newToken(jjmatchedKind, tokenImage);

   t.beginLine = beginLine;
   t.endLine = endLine;
   t.beginColumn = beginColumn;
   t.endColumn = endColumn;

   return t;
}

static int curLexState = 0;
static int defaultLexState = 0;
static int jjnewStateCnt;
static int jjround;
static int jjmatchedPos;
static int jjmatchedKind;

/** Get the next Token. */
public static Token getNextToken() 
{
  //int kind;
  Token specialToken = null;
  Token matchedToken;
  int curPos = 0;

  EOFLoop :
  for (;;)
  {   
   try   
   {     
      curChar = input_stream.BeginToken();
   }     
   catch(java.io.IOException e)
   {        
      jjmatchedKind = 0;
      matchedToken = jjFillToken();
      return matchedToken;
   }
   image = null;
   jjimageLen = 0;

   try { input_stream.backup(0);
      while (curChar <= 32 && (0x100000600L & (1L << curChar)) != 0L)
         curChar = input_stream.BeginToken();
   }
   catch (java.io.IOException e1) { continue EOFLoop; }
   jjmatchedKind = 0x7fffffff;
   jjmatchedPos = 0;
   curPos = jjMoveStringLiteralDfa0_0();
   if (jjmatchedKind != 0x7fffffff)
   {
      if (jjmatchedPos + 1 < curPos)
         input_stream.backup(curPos - jjmatchedPos - 1);
      if ((jjtoToken[jjmatchedKind >> 6] & (1L << (jjmatchedKind & 077))) != 0L)
      {
         matchedToken = jjFillToken();
         TokenLexicalActions(matchedToken);
         return matchedToken;
      }
      else
      {
         continue EOFLoop;
      }
   }
   int error_line = input_stream.getEndLine();
   int error_column = input_stream.getEndColumn();
   String error_after = null;
   boolean EOFSeen = false;
   try { input_stream.readChar(); input_stream.backup(1); }
   catch (java.io.IOException e1) {
      EOFSeen = true;
      error_after = curPos <= 1 ? "" : input_stream.GetImage();
      if (curChar == '\n' || curChar == '\r') {
         error_line++;
         error_column = 0;
      }
      else
         error_column++;
   }
   if (!EOFSeen) {
      input_stream.backup(1);
      error_after = curPos <= 1 ? "" : input_stream.GetImage();
   }
   throw new TokenMgrError(EOFSeen, curLexState, error_line, error_column, error_after, curChar, TokenMgrError.LEXICAL_ERROR);
  }
}

static void TokenLexicalActions(Token matchedToken)
{
   switch(jjmatchedKind)
   {
      case 31 :
        if (image == null)
            image = new StringBuffer();
        image.append(input_stream.GetSuffix(jjimageLen + (lengthOfMatch = jjmatchedPos + 1)));
                clike.printToken("identificador", matchedToken);
         break;
      default : 
         break;
   }
}
static private void jjCheckNAdd(int state)
{
   if (jjrounds[state] != jjround)
   {
      jjstateSet[jjnewStateCnt++] = state;
      jjrounds[state] = jjround;
   }
}
static private void jjAddStates(int start, int end)
{
   do {
      jjstateSet[jjnewStateCnt++] = jjnextStates[start];
   } while (start++ != end);
}
static private void jjCheckNAddTwoStates(int state1, int state2)
{
   jjCheckNAdd(state1);
   jjCheckNAdd(state2);
}

static private void jjCheckNAddStates(int start, int end)
{
   do {
      jjCheckNAdd(jjnextStates[start]);
   } while (start++ != end);
}

}
