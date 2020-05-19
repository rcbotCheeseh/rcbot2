#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

typedef float ga_nn_value;

//#define MAX (a,b) (((a)<(b))?(b):(a))

//#define scale (x,min,max) (((x)-(min))/((max)-(min)))
//#define descale (x,min,max) ((min)+((x)*((max)-(min))))

inline unsigned short int _MAX ( unsigned short int a, unsigned short int b )
{
	if ( a > b )
		return a;
	return b;
}

inline ga_nn_value zeroscale ( ga_nn_value x, ga_nn_value fMin, ga_nn_value fMax )
{
	return ((x-fMin)/(fMax-fMin));
}

// scales into -1 and +1 (medium at zero)
inline ga_nn_value gscale ( ga_nn_value x, ga_nn_value fMin, ga_nn_value fMax ) 
{ 
	return (zeroscale(x,fMin,fMax)*2)-1.0f;
}
// descales between 0 and 1 to min and max range
inline ga_nn_value gdescale ( ga_nn_value x, ga_nn_value fMin, ga_nn_value fMax )
{
	return (fMin+(x*(fMax-fMin)));
}

/*
inline ga_nn_value scale (ga_nn_value x, ga_nn_value min, ga_nn_value max)
{
	return ((x-min)/(max-min));
}

inline ga_nn_value descale (ga_nn_value x, ga_nn_value min, ga_nn_value max)
{
	return (min+(x*(max-min)));
}
*/
/*
// scale between -1 and 1
inline ga_nn_value scale (ga_nn_value x, ga_nn_value min, ga_nn_value max)
{
	static ga_nn_value zero_one;
	
	zero_one = (((x-min)/(max-min))*2)-1.0f;

	return (zero_one*2)-1.0f;
}

inline ga_nn_value descale (ga_nn_value x, ga_nn_value min, ga_nn_value max)
{
	//static ga_nn_value minus_one_to_one;
	
	//minus_one_to_one = (min+(x*(max-min)));

	return x;//((minus_one_to_one) + 1.0f)/2;
}
*/
class CNeuron
{
public:
	CNeuron ();

	CNeuron (unsigned short int iInputs);

	~CNeuron() { if ( m_inputs ) delete[] m_inputs; if ( m_weights ) delete[] m_weights; }

	void setWeights ( ga_nn_value *weights );

	virtual void input ( ga_nn_value *inputs );

	inline ga_nn_value getWeight ( unsigned short int i ) { return m_weights[i]; }

	ga_nn_value execute ();

	bool fired ();

	inline ga_nn_value getOutput () { return m_output; }

protected:
	
	unsigned short int m_iInputs;
	ga_nn_value m_LearnRate;
	ga_nn_value *m_inputs;
	ga_nn_value *m_weights;
	ga_nn_value m_output;
	ga_nn_value m_Bias;
	
};

class CPerceptron : public CNeuron
{
public:

	static ga_nn_value m_fDefaultLearnRate;// = 0.5f;
	static ga_nn_value m_fDefaultBias;// = 1.0f;

	CPerceptron (unsigned short int iInputs);

	void setWeights ( ga_nn_value *weights );

	ga_nn_value execute ();

	bool fired ();

	ga_nn_value getOutput ();

	void train ( ga_nn_value expectedOutput );

};

class CLogisticalNeuron : public CNeuron
{
public:
	CLogisticalNeuron()
	{
		m_error = 0;
		m_netinput = 0;
		m_LearnRate = 0.2f;
		m_output = 0;
		m_iInputs = 0;
		m_momentum = 0;
		m_Bias = -1.0f;
	}

	void init(unsigned short int iInputs, ga_nn_value learnrate);

	void train ();/// ITransfer *transferFunction, bool usebias = true );

	ga_nn_value execute ();//, bool usebias = true );

	inline void setError ( ga_nn_value err ) { m_error = err; }
	inline void addError ( ga_nn_value err ) { m_error += err; }
	inline void divError ( unsigned short int samples ) { m_error /= samples; }

	inline ga_nn_value getError ( unsigned short int w ) { return m_error * m_weights[w]; }
	inline ga_nn_value getMSE () { return m_error; }
private:
	ga_nn_value m_error;
	ga_nn_value m_netinput;
	ga_nn_value m_momentum;
};

typedef struct
{
	ga_nn_value *in;
	ga_nn_value *out;
}training_batch_t;

// manages training sets / (addition of)
class CTrainingSet
{
public:
	CTrainingSet( unsigned short int numInputs, unsigned short int numOutputs, unsigned short int numBatches )
	{
		m_numInputs = numInputs;
		m_numOutputs = numOutputs;
		m_numBatches = numBatches;

		init();
	}

	~CTrainingSet()
	{
		freeMemory();
	}

	void reset ()
	{
		freeMemory();
		init();
	}

	void freeMemory ()
	{
		if ( batches )
		{
			for ( unsigned short int i = 0; i < m_numBatches; i ++ )
			{
				delete[] batches[i].in;
				delete[] batches[i].out;
			}

			delete[] batches;
		}

		batches = NULL;
	}

	void init ()
	{
		m_batchNum = -1;
		m_fMin = 0;
		m_fMax = 1;
		m_inputNum = m_outputNum = 0;
		batches = new training_batch_t[m_numBatches];

		for ( unsigned short int i = 0; i < m_numBatches; i ++ )
		{
			batches[i].in = new ga_nn_value[m_numInputs];
			batches[i].out = new ga_nn_value[m_numOutputs];
			memset(batches[i].in,0,sizeof(ga_nn_value)*m_numInputs);
			memset(batches[i].out,0,sizeof(ga_nn_value)*m_numOutputs);
		}
	}

	inline void setScale ( ga_nn_value min, ga_nn_value max )
	{
		m_fMin = min;
		m_fMax = max;
	}

// input and scale between -1 and 1
	inline void in ( ga_nn_value input )
	{
		//assert(m_batchNum>=0);
		if ( (m_batchNum >= 0) && (m_batchNum < m_numBatches) && (m_inputNum < m_numInputs) ) 
			batches[m_batchNum].in[m_inputNum++] = scale(input);
	}

	// output and scale between 0 and 1
	inline void out ( ga_nn_value output )
	{
		//assert(m_batchNum>=0);
		if ( (m_batchNum >= 0) && (m_batchNum < m_numBatches) && (m_outputNum < m_numOutputs) ) 
			batches[m_batchNum].out[m_outputNum++] = zeroscale(output,m_fMin,m_fMax);
	}

	inline void addSet ( void )
	{
		if ( m_batchNum >= m_numBatches )
			return; // error -- too many

		m_batchNum++;
		m_inputNum = 0;
		m_outputNum = 0;
	}

	inline unsigned short int getNumBatches ()
	{
		return m_numBatches;
	}

	inline ga_nn_value scale ( ga_nn_value x ) 
	{ 
		return gscale(x,m_fMin,m_fMax);
	}

	inline ga_nn_value descale ( ga_nn_value x )
	{
		return gdescale(x,m_fMin,m_fMax);
	}

	inline ga_nn_value getMinScale () { return m_fMin; }
	inline ga_nn_value getMaxScale () { return m_fMax; }

	inline training_batch_t *getBatches () { return batches; }
private:
	// simple format (ins / outs)
	training_batch_t *batches;
	unsigned short int m_numInputs;
	unsigned short int m_numOutputs;
	unsigned short int m_numBatches;
	signed short int m_batchNum;
	unsigned short int m_inputNum;
	unsigned short int m_outputNum;

	ga_nn_value m_fMax; // maximum input/output value
	ga_nn_value m_fMin; // minumum input/output value
};

class CBotNeuralNet
{
public:

	CBotNeuralNet(unsigned short int numinputs, unsigned short int numhiddenlayers, unsigned short int neuronsperhiddenlayer, unsigned short int numoutputs, ga_nn_value learnrate);

	CBotNeuralNet ()
	{
		m_pOutputs = NULL;
		//m_transferFunction = NULL;

		m_numInputs = 0; // number of inputs
		m_numOutputs = 0; // number of outputs
		m_numHidden = 0; // neurons per hidden layer
		m_numHiddenLayers = 0;
		m_layeroutput = NULL;
		m_layerinput = NULL;
	}

	void execute ( ga_nn_value *inputs, ga_nn_value *outputs, ga_nn_value fMin, ga_nn_value fMax );

	void batch_train ( CTrainingSet *tset, unsigned short int epochs );

	~CBotNeuralNet ()
	{
		if ( m_pOutputs )
			delete [] m_pOutputs;
		//if ( m_transferFunction )
		//	delete m_transferFunction;
		if ( m_pHidden )
		{
			for ( unsigned short int i = 0; i < m_numHiddenLayers; i ++ )
				delete [] m_pHidden[i];
		}

		delete m_layerinput;
		delete m_layeroutput;
	}


private:
	//ITransfer *m_transferFunction;
	unsigned short int m_numInputs; // number of inputs
	unsigned short int m_numOutputs; // number of outputs
	unsigned short int m_numHidden; // neurons per hidden layer
	unsigned short int m_numHiddenLayers;

	CLogisticalNeuron *m_pOutputs;
	CLogisticalNeuron **m_pHidden;

	// used for passing values between layers
	ga_nn_value *m_layeroutput;
	ga_nn_value *m_layerinput;


};
#endif