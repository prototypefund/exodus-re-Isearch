/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.36
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class INFIX2RPN {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected INFIX2RPN(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(INFIX2RPN obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      IBJNI.delete_INFIX2RPN(swigCPtr);
    }
    swigCPtr = 0;
  }

  public INFIX2RPN() {
    this(IBJNI.new_INFIX2RPN(), true);
  }

  public String Parse(String InfixSentence) {
    return IBJNI.INFIX2RPN_Parse(swigCPtr, this, InfixSentence);
  }

  public boolean InputParsedOK() {
    return IBJNI.INFIX2RPN_InputParsedOK(swigCPtr, this);
  }

  public String GetErrorMessage() {
    return IBJNI.INFIX2RPN_GetErrorMessage(swigCPtr, this);
  }

}
