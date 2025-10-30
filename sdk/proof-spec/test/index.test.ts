import { ProofSpecClient, Phase, createProofSpecClient } from '../src/index';

// Mock ethers
jest.mock('ethers', () => {
  const originalModule = jest.requireActual('ethers');
  
  return {
    ...originalModule,
    Contract: jest.fn().mockImplementation((address, abi, signerOrProvider) => {
      const mockContract = {
        attest: jest.fn(),
        getAttestationCount: jest.fn(),
        filters: {
          Attested: jest.fn().mockReturnValue('attested-filter')
        },
        on: jest.fn(),
        off: jest.fn(),
        interface: {
          parseLog: jest.fn()
        }
      };
      return mockContract;
    })
  };
});

describe('ProofSpecClient', () => {
  let mockSigner: any;
  let mockProvider: any;
  const contractAddress = '0x1234567890123456789012345678901234567890';

  beforeEach(() => {
    jest.clearAllMocks();
    
    mockSigner = {
      getAddress: jest.fn().mockResolvedValue('0xactor'),
      signTransaction: jest.fn(),
      sendTransaction: jest.fn()
    };

    mockProvider = {
      getNetwork: jest.fn(),
      getBlockNumber: jest.fn()
    };
  });

  describe('constructor', () => {
    it('should create client with signer', () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);
      expect(client).toBeInstanceOf(ProofSpecClient);
    });

    it('should create client with provider', () => {
      const client = new ProofSpecClient(contractAddress, mockProvider);
      expect(client).toBeInstanceOf(ProofSpecClient);
    });
  });

  describe('attest', () => {
    it('should attest an action with LEARN phase', async () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);
      const mockContract = (client as any).contract;

      const mockReceipt = {
        hash: '0xtxhash',
        logs: [{
          address: contractAddress,
          topics: ['0xtopic']
        }]
      };

      const mockTx = {
        wait: jest.fn().mockResolvedValue(mockReceipt)
      };

      mockContract.attest.mockResolvedValue(mockTx);
      mockContract.interface.parseLog.mockReturnValue({
        name: 'Attested',
        args: {
          actor: '0xactor',
          action: 'Completed tutorial',
          phase: Phase.LEARN,
          timestamp: BigInt(1234567890)
        }
      });

      const result = await client.attest({
        action: 'Completed tutorial',
        phase: Phase.LEARN
      });

      expect(mockContract.attest).toHaveBeenCalledWith('Completed tutorial', Phase.LEARN);
      expect(result.action).toBe('Completed tutorial');
      expect(result.phase).toBe(Phase.LEARN);
      expect(result.transactionHash).toBe('0xtxhash');
    });

    it('should attest an action with SHIP phase', async () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);
      const mockContract = (client as any).contract;

      const mockReceipt = {
        hash: '0xtxhash2',
        logs: [{
          address: contractAddress,
          topics: ['0xtopic']
        }]
      };

      const mockTx = {
        wait: jest.fn().mockResolvedValue(mockReceipt)
      };

      mockContract.attest.mockResolvedValue(mockTx);
      mockContract.interface.parseLog.mockReturnValue({
        name: 'Attested',
        args: {
          actor: '0xactor',
          action: 'Deployed feature',
          phase: Phase.SHIP,
          timestamp: BigInt(1234567891)
        }
      });

      const result = await client.attest({
        action: 'Deployed feature',
        phase: Phase.SHIP
      });

      expect(result.phase).toBe(Phase.SHIP);
    });

    it('should attest an action with REST phase', async () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);
      const mockContract = (client as any).contract;

      const mockReceipt = {
        hash: '0xtxhash3',
        logs: [{
          address: contractAddress,
          topics: ['0xtopic']
        }]
      };

      const mockTx = {
        wait: jest.fn().mockResolvedValue(mockReceipt)
      };

      mockContract.attest.mockResolvedValue(mockTx);
      mockContract.interface.parseLog.mockReturnValue({
        name: 'Attested',
        args: {
          actor: '0xactor',
          action: 'Taking break',
          phase: Phase.REST,
          timestamp: BigInt(1234567892)
        }
      });

      const result = await client.attest({
        action: 'Taking break',
        phase: Phase.REST
      });

      expect(result.phase).toBe(Phase.REST);
    });

    it('should throw error if action is empty', async () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);

      await expect(
        client.attest({ action: '', phase: Phase.LEARN })
      ).rejects.toThrow('Action cannot be empty');

      await expect(
        client.attest({ action: '   ', phase: Phase.LEARN })
      ).rejects.toThrow('Action cannot be empty');
    });

    it('should throw error if phase is invalid', async () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);

      await expect(
        client.attest({ action: 'Test', phase: -1 as Phase })
      ).rejects.toThrow('Invalid phase value');

      await expect(
        client.attest({ action: 'Test', phase: 3 as Phase })
      ).rejects.toThrow('Invalid phase value');
    });

    it('should throw error if no signer provided', async () => {
      const client = new ProofSpecClient(contractAddress, mockProvider);

      await expect(
        client.attest({ action: 'Test', phase: Phase.LEARN })
      ).rejects.toThrow('Signer required for attestation');
    });
  });

  describe('getAttestationCount', () => {
    it('should return attestation count', async () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);
      const mockContract = (client as any).contract;

      mockContract.getAttestationCount.mockResolvedValue(BigInt(5));

      const count = await client.getAttestationCount('0xactor');

      expect(mockContract.getAttestationCount).toHaveBeenCalledWith('0xactor');
      expect(count).toBe(BigInt(5));
    });
  });

  describe('onAttested', () => {
    it('should listen for Attested events', () => {
      const client = new ProofSpecClient(contractAddress, mockSigner);
      const mockContract = (client as any).contract;

      const callback = jest.fn();
      const unsubscribe = client.onAttested(callback);

      expect(mockContract.filters.Attested).toHaveBeenCalled();
      expect(mockContract.on).toHaveBeenCalledWith('attested-filter', expect.any(Function));

      unsubscribe();
      expect(mockContract.off).toHaveBeenCalledWith('attested-filter', expect.any(Function));
    });
  });

  describe('createProofSpecClient', () => {
    it('should create a client instance', () => {
      const client = createProofSpecClient(contractAddress, mockSigner);
      expect(client).toBeInstanceOf(ProofSpecClient);
    });
  });
});
