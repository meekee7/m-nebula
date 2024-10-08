#ifndef N_SCRIPTSERVER_H
#define N_SCRIPTSERVER_H
//--------------------------------------------------------------------
/**
 *   @defgroup ScriptServices Scripting Services
 *     @ingroup NebulaKernelModule
 */
//--------------------------------------------------------------------
/**
    @class nScriptServer
    @ingroup ScriptServices

    The script server object is the communication point between
    a nRoot object's Dispatch() method and a specific scripting
    language. More specifically, the script server's main task is to
    translate nCmd objects into script statements and back, and to
    communicate with receiver nRoot objects by sending nCmd objects
    to them (which is done by involing Dispatch() on the receiver
    with the nCmd object as the argument).

    Script servers are also used as filter for the object serialization.
    An object serializes itself by emitting nCmd objects to the
    file server. The file server will talk to a script server to
    translate those nCmd objects to actual script statements,
    which are then written to the output stream. This stream
    must then simply be fed into a script server to reconstruct
    the serialized object, no special loader code is needed.

    (C) 2002 RadonLabs GmbH
*/
#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nScriptServer
#include "kernel/ndefdllclass.h"

//------------------------------------------------------------------------------
class nFile;
class N_PUBLIC nScriptServer : public nRoot 
{
public:
    // the select methods (see below)
    enum SelectMethod 
    {
        SELCOMMAND,         // write a select statement after object constructor
        NOSELCOMMAND,       // don't write a select statement
    };

    /// constructor
    nScriptServer();
    /// destructor
    virtual ~nScriptServer();
    /// evaluate a script command
    virtual bool Run(const char *, const char*&); 
    /// evaluate a script file
    virtual bool RunScript(const char *, const char*&);
	/// evaluate a script file
	virtual bool RunScriptFS(const char *, const char*&);
    /// write header of a persistent object file
    virtual nFile* BeginWrite(const char* filename, nRoot* obj);
    /// write a comment to the file
    virtual bool WriteComment(nFile *, const char *);
    /// write object header with default constructor
    virtual bool WriteBeginNewObject(nFile* fp, nRoot *o, nRoot *owner);
    /// write object header with constructor cmd 
    virtual bool WriteBeginNewObjectCmd(nFile* fp, nRoot *o, nRoot *owner, nCmd *cmd);  
    /// write object header without constructor (select only)
    virtual bool WriteBeginSelObject(nFile* fp, nRoot *o, nRoot *owner); 
    /// translate and write a cmd message
    virtual bool WriteCmd(nFile*, nCmd *);   
    /// write object footer
    virtual bool WriteEndObject(nFile* fp, nRoot *o, nRoot *owner);
    /// finish a persistent object file
    virtual bool EndWrite(nFile*);
	/// get script command begin symbols
	virtual const char* GetCmdBegin() const;
	/// get script command end symbols
	virtual const char* GetCmdEnd() const;
	/// get script command parameters begin symbols
	virtual const char* GetCmdParamBegin() const;
	/// get script command parameters delimiter
	virtual const char* GetCmdParamDelim() const;
	/// get script command parameters end symbols
	virtual const char* GetCmdParamEnd() const;	
	/// get extensions supported by script server
	const strings_v& GetExtensions() const { return this->extensions;}
	/// check if extension supported by script server
	bool IsExtSupported(const char* ext) const;
    /// abort program on script error?
    void SetFailOnError(bool b);
    /// get abort program status
    bool GetFailOnError() const;
    /// set the select method when writing persistent objects
    void SetSelectMethod(SelectMethod sm);
    /// get current select method
    SelectMethod GetSelectMethod() const;
    /// set the quit requested flag
    void SetQuitRequested(bool b);
    /// get the quit requested flag
    bool GetQuitRequested() const;	

    /// trigger server (should be called frequently)
    virtual bool Trigger(void);

#ifndef __XBxX__
    /// generate a prompt string for interactive mode
    virtual char *Prompt(char *, int); 
#endif

    static nKernelServer* kernelServer;
    static nClass* local_cl;

protected:
	void write_select_statement(nFile *, nRoot *, nRoot *);

protected:
	strings_v		extensions; ///< extensions list

	long			indent_level;
    stl_string		indent_buf;

private:
    bool			quitRequested;
    bool			failOnError;
    SelectMethod	selMethod;	
};



//------------------------------------------------------------------------------
/**
*/
inline
bool
nScriptServer::GetQuitRequested() const
{
    return this->quitRequested;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nScriptServer::SetFailOnError(bool b)
{
    this->failOnError = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nScriptServer::GetFailOnError() const
{
    return this->failOnError;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nScriptServer::SetSelectMethod(nScriptServer::SelectMethod sm)
{
    this->selMethod = sm;
}

//------------------------------------------------------------------------------
/**
*/
inline
nScriptServer::SelectMethod
nScriptServer::GetSelectMethod() const
{
    return this->selMethod;
}

//------------------------------------------------------------------------------
/**
*/
inline
void nScriptServer::write_select_statement(nFile *, nRoot *, nRoot *)
{
	n_error("write_select_statement: pure virtual function called\n");
}

//------------------------------------------------------------------------------
#endif
