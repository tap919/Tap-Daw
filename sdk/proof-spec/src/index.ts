import { ethers, Contract, Signer, Provider } from 'ethers';

/**
 * Phase enum representing different stages
 */
export enum Phase {
  LEARN = 0,
  SHIP = 1,
  REST = 2
}

/**
 * Interface for attesting actions
 */
export interface AttestParams {
  action: string;
  phase: Phase;
}

/**
 * Result of an attestation
 */
export interface AttestResult {
  transactionHash: string;
  actor: string;
  action: string;
  phase: Phase;
  timestamp: bigint;
}

/**
 * ProofOfAction contract ABI
 */
const PROOF_OF_ACTION_ABI = [
  "function attest(string memory action, uint8 phase) external",
  "function getAttestationCount(address actor) external view returns (uint256)",
  "event Attested(address indexed actor, string action, uint8 phase, uint256 timestamp)"
];

/**
 * ProofSpec SDK client for interacting with ProofOfAction smart contract
 */
export class ProofSpecClient {
  private contract: Contract;
  private signer: Signer;

  /**
   * Create a new ProofSpec client
   * @param contractAddress - Address of the deployed ProofOfAction contract
   * @param signerOrProvider - Ethers signer or provider
   */
  constructor(contractAddress: string, signerOrProvider: Signer | Provider) {
    if ('getAddress' in signerOrProvider) {
      // It's a Signer
      this.signer = signerOrProvider as Signer;
      this.contract = new Contract(contractAddress, PROOF_OF_ACTION_ABI, this.signer);
    } else {
      // It's a Provider - create a read-only contract
      this.contract = new Contract(contractAddress, PROOF_OF_ACTION_ABI, signerOrProvider);
      this.signer = null as any;
    }
  }

  /**
   * Attest an action with a specific phase
   * @param params - Attestation parameters including action and phase
   * @returns Promise resolving to attestation result
   */
  async attest(params: AttestParams): Promise<AttestResult> {
    if (!this.signer) {
      throw new Error('Signer required for attestation');
    }

    const { action, phase } = params;

    if (!action || action.trim().length === 0) {
      throw new Error('Action cannot be empty');
    }

    if (phase < Phase.LEARN || phase > Phase.REST) {
      throw new Error('Invalid phase value');
    }

    const tx = await this.contract.attest(action, phase);
    const receipt = await tx.wait();

    // Parse the Attested event from the receipt
    const attestedEvent = receipt.logs
      .map((log: any) => {
        try {
          return this.contract.interface.parseLog(log);
        } catch {
          return null;
        }
      })
      .find((event: any) => event && event.name === 'Attested');

    if (!attestedEvent) {
      throw new Error('Attested event not found in transaction receipt');
    }

    return {
      transactionHash: receipt.hash,
      actor: attestedEvent.args.actor,
      action: attestedEvent.args.action,
      phase: attestedEvent.args.phase,
      timestamp: attestedEvent.args.timestamp
    };
  }

  /**
   * Get the attestation count for an address
   * @param address - Address to check
   * @returns Promise resolving to the attestation count
   */
  async getAttestationCount(address: string): Promise<bigint> {
    return await this.contract.getAttestationCount(address);
  }

  /**
   * Listen for Attested events
   * @param callback - Callback function to handle events
   * @returns Function to stop listening
   */
  onAttested(callback: (event: AttestResult) => void): () => void {
    const filter = this.contract.filters.Attested();
    
    const listener = (actor: string, action: string, phase: number, timestamp: bigint, event: any) => {
      callback({
        transactionHash: event.log.transactionHash,
        actor,
        action,
        phase,
        timestamp
      });
    };

    this.contract.on(filter, listener);

    return () => {
      this.contract.off(filter, listener);
    };
  }
}

/**
 * Create a ProofSpec client instance
 * @param contractAddress - Address of the deployed ProofOfAction contract
 * @param signerOrProvider - Ethers signer or provider
 * @returns ProofSpec client instance
 */
export function createProofSpecClient(
  contractAddress: string,
  signerOrProvider: Signer | Provider
): ProofSpecClient {
  return new ProofSpecClient(contractAddress, signerOrProvider);
}
